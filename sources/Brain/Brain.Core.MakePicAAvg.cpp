#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




//-----------------------------------------------------------------------------------------------
// Картина поля активности, усредненного по времении (этот метод не используется)
/*
void Brain::Core::MakePicAAvg() {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = this->Cortex[ix, iy].AAccumSinapse * 255; //AAccumSinapse - накопленная за время обучения синапсов активность
			this->picAAvg->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина поля активности, усредненного по времении (этот метод не используется). MakePicAAvg()
*/