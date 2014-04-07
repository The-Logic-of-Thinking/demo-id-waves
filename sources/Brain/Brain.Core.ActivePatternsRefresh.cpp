#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ���������� ������� �������� ������� �������
void Brain::Core::ActivePatternsRefresh() {
	for (int item = 0; item < this->NP; item++) {	
		for (int ix = 0; ix <= 2 * this->R_Pat; ix++) {
			for (int iy = 0; iy <= 2 * this->R_Pat; iy++) {
				if (this->PatSet[item].P[ix, iy] == 1) {
					// ������� �.�.: ���� ������ ������� ������ ��� ������� �����, �� �� ����� �� �� ��� ��, ��:
					if (this->CheckedPatterns->Contains(item) && this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status != 6) {
						this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 6; // ������ ��� ������� ������� (�������) (6)
					}
					// ������� �.�.: ���� ������ ������� �� ������ ��� ������� �����, �� �� ����� �� ��� ��, ��:
					else if (!this->CheckedPatterns->Contains(item) && this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status == 6) {
						if (this->CT != 0) {
							this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 4; // ��������� � ��������� ���������� (�����) (4)
						}
						else { // ������� �.�.: ��� ������, ����� ������ ������ �������������:
							this->Cortex[this->PatSet[item].ix - this->R_Pat + ix, this->PatSet[item].iy - this->R_Pat + iy].Status = 0; // ��������� � ��������� ����� (������) (0)
						}
					}
					// ������� �.�.: � ��������� ������� ������ �� ��������
				}
			}
		}
	}
				
	// ������� �.�.: ������������ �������� ��������
	this->Draw_Status();
	// ������� �.�.: ���������� ������ ������� �������
	this->Draw_Numbers();
} // ���������� ������� �������� ������� �������. ActivePatternsRefresh()