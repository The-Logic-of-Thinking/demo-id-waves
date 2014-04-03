#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������� ���� ���������� (��������)
void Brain::Core::Draw_FieldA() {
	int Y; // ���� � R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A[ix, iy] / 0.08 * 255 ); // Field_A - ���� ���������� ���������� ����
			if (Y > 255) {
				Y = 255;
			}
			this->picFieldA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ���� ���������� (��������). Draw_FieldA()