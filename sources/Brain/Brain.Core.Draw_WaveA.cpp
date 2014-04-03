#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Картина поля волны (размытая)
void Brain::Core::Draw_WaveA() {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Wave[ix, iy] / 0.04 * 255 ); // Field_A_Wave - поле волновой активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picWaveA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина поля волны (размытая). Draw_WaveA()