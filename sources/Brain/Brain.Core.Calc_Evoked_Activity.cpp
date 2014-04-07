#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������ ��������� ����������
void Brain::Core::Calc_Evoked_Activity() {
	float x1;
	float x2;
	float sum;
	float n1;
	float n2;

	for (int ix = this->R_W; ix < this->X_C - this->R_W; ix++) {
		for (int iy = this->R_W; iy < this->Y_C - this->R_W; iy++) {
			this->Cortex[ix, iy].EvokedA = 0; // ��������� ��������� ���������� ������� � ���������� ���������
			if (this->Cortex[ix, iy].StatusL > 0) { // ������� ������ �������� �������
				// 0 - ��������; 1 - �����������; 2 - ������; 5 - �� �������� �������� (������� �����)
				//
				// ������� �.�.: �� ���, ���� ���� ������� �� �������� �������� 5, ������� �� �������� ��������?
				//
				sum = 0;
				n1 = 0;
				n2 = 0;

				for (int ix1 = 0; ix1 <= 2 * this->R_W; ix1++) {
					for (int iy1 = 0; iy1 <= 2 * this->R_W; iy1++) {
						x1 = this->Cortex[ix + ix1 - this->R_W, iy + iy1 - this->R_W].AAccumSinapse;
						x2 = this->CortexW[ix, iy, ix1, iy1];

						sum += x1 * x2;

						n1 += x1;
						n2 += x2;
					}
				}

				if (sum > 0) {
					this->Cortex[ix, iy].EvokedA = (float)( sum / System::Math::Sqrt(n1 * n2) );
				}

				/*
				if (this->Cortex[ix, iy].EvokedA > this->L_Spike) {
					if (this->Cortex[ix, iy].Status != 1) // 1 - ��������� ���������� // ����������� (������� �.�.): �������
						this->Cortex[ix, iy].T = this->CT + this->T_EnvokedA;
					this->Cortex[ix, iy].Status = 1;
				}
				*/
				// ������� �.�.: ���� ����������� ����� ���:
				if (this->Cortex[ix, iy].EvokedA > this->L_Spike && this->Cortex[ix, iy].Status != 1) {
					this->Cortex[ix, iy].T = this->CT + this->T_EnvokedA;
					this->Cortex[ix, iy].Status = 1; // 1 - ��������� ���������� // ����������� (������� �.�.): �������
				}
			}
		}
	}
} // ������ ��������� ����������. Calc_Evoked_Activity()