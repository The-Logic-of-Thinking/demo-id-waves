#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Картина накопленной активности
void Brain::Core::Draw_ActAccum() {
	int Y; // Цвет в R=G=B
	
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Accum[ix, iy] * 500 ); // Field_A_Accum - поле накопленной вызванной активности коры
			if (Y > 255) {
				Y = 255;
			}
			this->picActAccum->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина накопленной активности. Draw_ActAccum()