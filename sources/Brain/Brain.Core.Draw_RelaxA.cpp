#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// Картина релаксации
void Brain::Core::Draw_RelaxA(int ntype) {
	int Y; // Цвет в R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Relax[ix, iy, ntype] / 0.04 * 255 ); // Field_A_Relax - поле плотности релаксирующих нейронов каждого из типов
			if (Y > 255) {
				Y = 255;
			}
			this->picRelaxA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // Картина релаксации. Draw_RelaxA(int ntype)