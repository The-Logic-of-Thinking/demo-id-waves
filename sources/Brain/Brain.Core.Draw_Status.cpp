#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Картина общего состояния коры (статус нейронов)
void Brain::Core::Draw_Status() {
	System::Drawing::Pen ^Pen;
	int i1;
	int i2;
	
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			switch (this->Cortex[i, j].Status) {
				case 0: // Покой // 0 - спокоен // Комментарии (Егармин К.Н.): черный
					this->pic->SetPixel(i, j, System::Drawing::Color::Black);
					break;
				case 1: // Вызванная активность // 1 - вызванная активность // Комментарии (Егармин К.Н.): красный
					this->pic->SetPixel(i, j, System::Drawing::Color::Orange);
					break;
				case 2: // Спайки волны идентификатора // 2 - волновая активность // Комментарии (Егармин К.Н.): белый
					this->pic->SetPixel(i, j, System::Drawing::Color::Gray);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 3: // Фронт волны идентификатора // 3 - первый такт волновой активности
					this->pic->SetPixel(i, j, System::Drawing::Color::White);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 4: // Нейроны в заторможенном состоянии // 4 - релаксация после волнового спайка // Комментарии (Егармин К.Н.): синий
					this->pic->SetPixel(i, j, System::Drawing::Color::DarkBlue);
					break;
				case 5: // Истинно спонтанный спайк // 5 - спонтанная активность // Комментарии (Егармин К.Н.): зеленый
					this->pic->SetPixel(i, j, System::Drawing::Color::Green);
					break;
				case 6: // Проекционная активность // 6 - входной аксон
					this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					break;
			}
		}
	}

	// Если есть волновой туннель, то рисование его границ
	// if ((this->Text == "Form1" && cortex2 != nullptr) || this->Text == "2")
	// Егармин К.Н.: думаю лучше так:
	if ( (this->cortex2 != nullptr) != (this->cortex0 != nullptr) ) {
		Pen = gcnew System::Drawing::Pen(System::Drawing::Color::White, 1);
		
		if ( this->cortex2 != nullptr ) {
			i1 = this->TX1;
			i2 = this->TY1;
		}
		else
		{
			i1 = this->TX2;
			i2 = this->TY2;
		}
		
		this->Gpic->DrawLine(Pen, i1, i2, i1 + TD, i2);
		this->Gpic->DrawLine(Pen, i1, i2 + TD, i1 + TD, i2 + TD);
		this->Gpic->DrawLine(Pen, i1, i2, i1, i2 + TD);
		this->Gpic->DrawLine(Pen, i1 + TD, i2, i1 + TD, i2 + TD);
	}
} // Картина общего состояния коры (статус нейронов). Draw_Status()