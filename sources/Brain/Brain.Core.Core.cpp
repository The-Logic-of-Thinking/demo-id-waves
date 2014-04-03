#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"



//
// Конструктор коры
//
Brain::Core::Core() {
	// Подготовка генератора случайных чисел от 0 до 1
	this->rnd = gcnew System::Random();
	
	this->InitCore();
	
	this->SetSens();
	
	this->PatCreate();
	
	// Инициация нейронов коры
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Cortex[ix, iy].Type = 0;
			// Комментарии (Егармин К.Н.): 0 - входной аксон
			// Изначально все аксоны входные
			this->Cortex[ix, iy].ID = gcnew System::Collections::ObjectModel::Collection<ID>();
		}
	}

	// Создание шаблона распрстранения активности
	// Сумма всех точек дает единицу. Это позволяет устанавливать пороги для полей в процентах задействованных нейронов
	// Комментарии (Егармин К.Н.):
	// Здесь речь о белых размытых пятнах на картинке слева внизу, а точнее:
	// описывается область, на которую влияет нейрон при срабатывании
	float dx = 0;
	float dy = 0;
	for (int ix = 0; ix < this->NLocalAM; ix++) {
		for (int iy = 0; iy < this->NLocalAM; iy++) {
			dx = (float)(ix - this->RLAM);
			dy = (float)(iy - this->RLAM);
			this->LocalAM[ix, iy] = (float)( (1 / (2 * System::Math::PI * this->Sigma2LAM)) * System::Math::Exp( -(System::Math::Pow(dx, 2) + System::Math::Pow(dy, 2)) / (2 * this->Sigma2LAM) ) );
		}
	}
}