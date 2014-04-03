#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������ ����� ��������� ����
void Brain::Core::CalcFields() {
	int x = 0;
	int y = 0;
	int T = 0;
	float s = 0;

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			// ���� ����������� ��������� ���������� ����
			this->Field_A_Accum[ix, iy] = this->Field_A_Accum[ix, iy] * this->KA;
			// ���� ���������� ���������� ����
			this->Field_A[ix, iy] = 0;
			// ���� �������� ���������� ����
			this->Field_A_Wave[ix, iy] = 0;
			// ���� ��������� ���������� ����
			this->Field_A_Evoced[ix, iy] = 0;

			for (int i = 0; i < this->N_Types; i++) {
				// ���� ��������� ������������� �������� ������� �� �����
				this->Field_A_Relax[ix, iy, i] = 0;
			}
		}
	}

	for (int ix = this->RLAM; ix < this->X_C - this->RLAM; ix++) {
		for (int iy = this->RLAM; iy < this->Y_C - this->RLAM; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				// 0 - ������� // ����������� (������� �.�.): ������
				// 7 - ��������� �������� �� ��������� ������� � �������� ��������������

				case 1: // 1 - ��������� ���������� // ����������� (������� �.�.): �������
				case 6: // 6 - ������� �����
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];
							
							// ���� ����������� ��������� ���������� ����
							this->Field_A_Accum[x, y] += s;
							// ���� ���������� ���������� ����
							this->Field_A[x, y] += s;
							// ���� ��������� ���������� ����
							this->Field_A_Evoced[x, y] += s;
						}
					}
					break;
				
				case 2: // 2 - �������� ���������� // ����������� (������� �.�.): �����
				case 3: // 3 - ������ ���� �������� ����������
				case 5: // 5 - ���������� ���������� // ����������� (������� �.�.): �������
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];

							// ���� ���������� ���������� ����
							this->Field_A[x, y] += s;
							// ���� �������� ���������� ����
							this->Field_A_Wave[x, y] += s;
						}
					}
					break;

				case 4: // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
					T = this->Cortex[ix, iy].Type;
					// ����������� (������� �.�.):
					// T = 0 - ������� �����
					// T = 1 - ������ ������� ������ (��������)
					// T = 2 - ������ ������� ������
					for (int ix1 = 0; ix1 < this->NLocalAM; ix1++) {
						for (int iy1 = 0; iy1 < this->NLocalAM; iy1++) {
							x = ix + ix1 - this->RLAM;
							y = iy + iy1 - this->RLAM;

							s = this->LocalAM[ix1, iy1];

							// ���� ��������� ������������� �������� ������� �� �����
							this->Field_A_Relax[x, y, T] += s;
							//
							// ����������� (������� �.�.):
							//
							// ����������, ��� ������������� ������ ������ ������ �� ����� ��������� �� ����?
						}
					}
					break;
			}
		}
	}
} // ������ ����� ��������� ����. CalcFields()