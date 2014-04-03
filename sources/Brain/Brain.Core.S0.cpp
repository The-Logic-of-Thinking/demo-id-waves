#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ��������� ������� � ��������� �����
void Brain::Core::S0(int %ix, int %iy) {
	int sum;
	int NN;
	//int NP;
	int P;
	
	// �������, ���� ������� ���������� ������ ������� ��� � �� ������� ��� ��������� ����
	if (this->Field_A[ix, iy] < this->LAmin) {
		return;
	}

	// ���� ������� ���������� ���������� �����, �� ��������� �� ������������� ��������� �����������
	if (this->Cortex[ix, iy].ID->Count > 0) {
		for each (ID S in this->Cortex[ix, iy].ID) {
			sum = 0;
			NN = 0;
			//NP = 0;

			for (int ix1 = ix - this->R_ID; ix1 <= ix + this->R_ID; ix1++) {
				for (int iy1 = iy - this->R_ID; iy1 <= iy + this->R_ID; iy1++) {
					P = S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)];
					// ����������� ����������� ����� ���������� � ����������
					/*
					sum += this->Cortex[ix1, iy1].AAccumExtraSinapse * 
						   P * 
						   this->SensTable[this->Cortex[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)].Type, this->Cortex[ix, iy].Type, 0];
				 	*/
					// ������� �.�.: ��� �������, ��� ����� ������ ���� ���:
					sum += (int)( this->Cortex[ix1, iy1].AAccumExtraSinapse * 
								  P * 
								  this->SensTable[this->Cortex[ix1, iy1].Type, this->Cortex[ix, iy].Type, 0] );
								  // ������� �.�.: ������� ������ ���������������� �������� (0)
					NN += P;
				}
			}

			if ( (sum / (double)NN) > this->L_act ) {
				// ���� ��������� �������� ����������� ����������� � ������������� ���
				if (S.ConsStatus == 0) { // 0 - �� ��������������
					S.NAct += 1;
					if (S.NAct > this->NCons) { // NCons - ���������� ����, ����������� ��� ������������
						S.ConsStatus = 1; // 1 - ��������������
					}
				}

				this->Cortex[ix, iy].Status = 3; // 3 - ������ ���� �������� ����������
				
				// T - ��������� �������
				// CT - ������� ����
				// T_ID - ����������������� ������ ����������������� ����������
				this->Cortex[ix, iy].T = this->CT + this->T_ID;

				// ��� ��������� ����� ����� �� �����, �� ��� ����� �������� ������������ ��������� ����� 
				
				//
				// ������� �.�.:
				// �� ����� ������� ��� ���� ���, �� ���-����� ���� �������� ����, ���, ����
				// ������ ������ ���� ������� ��������� ����������, �� � ��� ��� ������ ������������� ��
				// ������ ���������.
				// ����� ��� ��������� �� ����, ��� ����� �� ���� ��������, �� ����� ��� ��� ������,
				// ���� �� �� ����� ������������� ����� ������ ������� ������ ��������� ��������� ����������
				//
				if (this->CheckedPatterns->Count == 1 && this->CheckedPatterns[0] != S.NP) {
					this->Cortex[ix, iy].ERR = true;
				}
			}
		} // for each
	}
} // ��������� ������� � ��������� �����. S0(int %ix, int %iy)