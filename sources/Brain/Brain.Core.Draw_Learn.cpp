#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������� ��������� ��������
void Brain::Core::Draw_Learn() {
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			switch (this->Cortex[ix, iy].StatusL) {
				case 0: // 0 - ��������
					this->pic1->SetPixel(ix, iy, System::Drawing::Color::Black);
					break;
				case 1: // 1 - �����������
					if (this->Cortex[ix, iy].Status == 1) { // 1 - ��������� ���������� // ����������� (������� �.�.): �������
						// Field_A_Accum - ���� ����������� (����������) ��������� ���������� ����
						if (this->Field_A_Accum[ix, iy] < this->L_GameLive_Low) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Blue);
						}
						else if (this->Field_A_Accum[ix, iy] >= this->L_GameLive_Low && 
								 this->Field_A_Accum[ix, iy] < this->L_GameLive_high) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Green);
						}
						else if (this->Field_A_Accum[ix, iy] >= this->L_GameLive_high) {
							this->pic1->SetPixel(ix, iy, System::Drawing::Color::Red);
						}
					}
					else {
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::Yellow);
					}
					break;
				case 2: // 2 - ������
					if (this->Cortex[ix, iy].Status == 1) { // 1 - ��������� ���������� // ����������� (������� �.�.): �������
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::White);
					}
					else {
						this->pic1->SetPixel(ix, iy, System::Drawing::Color::DarkBlue);
					}
					break;
				case 5: // 5 - �� �������� �������� (������� �����)
					this->pic1->SetPixel(ix, iy, System::Drawing::Color::DarkMagenta); // ������� �.�.: ���������� - ���� ������ �������� ��������� �������� ��������� ����������
					break;
			}
		}
	}
} // ������� ��������� ��������. Draw_Learn()