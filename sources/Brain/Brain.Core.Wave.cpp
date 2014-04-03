#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Создать один такт волны
void Brain::Core::Wave() {
	double AW;
	int k;
	int sum;

	// Расчет полей состояния коры
	this->CalcFields();

	/*
	// Комментарии (Егармин К.Н.): этот блок перенесен в отдельную функцию в объекте (модуле) Формы
	// Отображение выбранного поля активности
	switch (ComboBox1->SelectedIndex) {
		case 0:
			Draw_ActAccum(); // акт. накопленная (размытая)
			break;
		case 1:
			Draw_FieldA(); // акт. моментальная (размытая)
			break;
		case 2:
			Draw_EvokedA(); // акт. вызванная
			break;
		case 3:
			Draw_WaveA(); // акт. волновая
			break;
		case 4:
			Draw_RelaxA(); // уровень релаксации
			break;
	}
	*/

	// Определяем переходы состояний нейронов
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 0: // покой // 0 - спокоен // Комментарии (Егармин К.Н.): черный
					// определяем узнавание идентификатора, но не корректируем CortexA, а только меняем статус
					this->S0(ix, iy);
					
					// Если возбуждения нет
					if (this->Cortex[ix, iy].Status == 0) {
						// уровень окружающей активности высок, но еще не достиг верхнего порога, 
						if (this->Field_A[ix, iy] > this->LAmin && // Поле мгновенной активности коры
							this->Field_A_Wave[ix, iy] < this->LAmax && // Поле волновой активности коры
							this->Field_A_Relax[ix, iy, this->Cortex[ix, iy].Type] < this->LImin[this->Cortex[ix, iy].Type]) { // Поле плотности релаксирующих нейронов каждого из типов
							// а блокированных нейронов вокруг мало, то
							if (this->rnd->NextDouble() < this->Psp) {
								// создаем кандидата на добавление в идентификатор
								this->Cortex[ix, iy].Status = 7; // 7 - состояние проверки на возможное участие в волновом идентификаторе
								this->Cortex[ix, iy].T = this->CT + this->T_ExtraSinapseLearn;
							}
						}
					}
					break;
				case 7: // 7 - состояние проверки на возможное участие в волновом идентификаторе
					// определяем узнавание идентификатора, но не корректируем CortexA, а только меняем статус
					this->S0(ix, iy);
					// Егармин К.Н.: если статус не изменился
					if (this->Cortex[ix, iy].Status == 7) {
						if (this->Field_A_Wave[ix, iy] > this->LAmax) {// Поле волновой активности коры
							// Сбрасываем состояние кандидата, если активность превышает порог
							this->Cortex[ix, iy].Status = 0;
						}
						else if (this->CT > this->Cortex[ix, iy].T) {
							// вызываем спонтанную активность и проводим ID обучение 
							this->Cortex[ix, iy].Status = 5; // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
							this->Cortex[ix, iy].T = this->CT;
							// Создание метаботропного рецептивного кластера
							this->S1(ix, iy);
						}
					}
					break;
				case 1: // вызванная активность // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
					// Сбрасываем вызванную активность по окончании пакета
					if (this->CT > this->Cortex[ix, iy].T) { // Егармин К.Н.: если нейрон был возбужден, то теперь может перейти в состояние покоя
						this->Cortex[ix, iy].Status = 0;
					}
					break;
				case 2: // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					// если пачка закончилась, то переводим в состояние нечувствительности
					if (this->CT > this->Cortex[ix, iy].T) {
						// задаем время релаксации							 // тем больше, чем сильнее рядом вызванная активность
						this->Cortex[ix, iy].T = this->CT + this->T_Passive; // * (1 + Field_A_Accum(ix, iy) / 0.02)
						this->Cortex[ix, iy].Status = 4; // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
						// Егармин К.Н.: про ошибки я вообще не понимаю
						this->Cortex[ix, iy].ERR = false;
					}
					break;
				case 3: // первый такт волновой активности // 3 - первый такт волновой активности
					this->Cortex[ix, iy].Status = 2; // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					break;
				case 4: // состояние нечувствительности // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
					if (this->CT > this->Cortex[ix, iy].T) { // Егармин К.Н.: если релаксация прошла, то
						this->Cortex[ix, iy].Status = 0;     // переходим в состояние покоя
					}
					break;
				case 5: // спонтанный спайк // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
					this->Cortex[ix, iy].Status = 2; // пачка волновой активности // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					break;
				//case 6: break; // Проекционная (входная) активность // 6 - входной аксон
			}
		}
	}

	// Создаем картину активности нового временного слоя, соответсвующую статусам
	AW = 0; // тип double
	// Сдвигаем текщий временной слой
	this->CurLayer += 1;
	if (this->CurLayer == this->T_SinapseLearn) {
		this->CurLayer = 0;
	}
	
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 1: // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
				case 2: // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
				case 3: // 3 - первый такт волновой активности
				case 5: // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
				case 6: // 6 - входной аксон
					this->CortexA[ix, iy, this->CurLayer] = 1; // Набор полей последних состояний нейронов.
					AW += 1;
					break;
				default: // 0 - спокоен // Комментарии (Егармин К.Н.): черный
						 // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
						 // 7 - состояние проверки на возможное участие в волновом идентификаторе
					this->CortexA[ix, iy, this->CurLayer] = 0; // Набор полей последних состояний нейронов.
					break;
			}
		}
	}

	// График ритма
	this->CurPosA += 1;
	if (this->CurPosA == this->NSer) {
		this->CurPosA = 0;
	}
	// Суммарная ритмическая активность
	this->AWave[this->CurPosA] = AW / ((this->X_C - 2 * this->R_ID) * (this->Y_C - 2 * this->R_ID));
	
	// График суммарного ритма
	this->DrawChart();
	// Проверяем на консолидацию или удаление идентификационных следов
	// Егармин К.Н.: перерисовать основную картинку
	this->Draw_Status();

	this->CT += 1;

	if (this->FPic) {
		this->addpic();
	}

	// Установка последовательности слоев (0 - текущий, 1 - предыдущий и т.д.)
	k = this->CurLayer; // тип int
	for (int i = 0; i < this->T_SinapseLearn; i++) {
		this->LayersSequence[i] = k;
		k -= 1;
		if (k < 0) {
			k = T_SinapseLearn - 1;
		}
	}

	// Расчет накопленной по времени активности
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			sum = 0; // тип int
			
			// Егармин К.Н.: похоже, что AAccumExtraSinapse и AAccumSinapse - просто флаги того, что
			// последние T_ExtraSinapseLearn и T_SinapseLearn (T_SinapseLearn > T_ExtraSinapseLearn)
			// тактов, соответственно, нейрон находился хоть в каком-то поле активности.
			//
			// Егармин К.Н.: у Редозубова почему-то эти флаги выставлены вещественными числами,
			// а не Byte, как он обычно делает для флагов. Почему?
			
			/*
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++)
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumSinapse = 0;
			*/

			// Егармин К.Н.: код выше несколько не эффективен, лучше записать так:
			this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			this->Cortex[ix, iy].AAccumSinapse = 0;
			
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
				this->Cortex[ix, iy].AAccumSinapse = 1;
				continue;
			}									
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumSinapse = 1;
			}
		}
	}
	
	// Обучение
	if (this->FLearn) {
		// Расчет вызванной активности
		this->Calc_Evoked_Activity();

		//this->MakePicFieldA();

		// Обучение синаптических весов. Задание фасилитации
		this->LearnW1();
		
		// Отмена фасилитации, исходя из условий выживания
		// this->LearnW2();
	}

	// Проекция по волновому туннелю
	this->TMove();
} // Создать один такт волны. Wave()