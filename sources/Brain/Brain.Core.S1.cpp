#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// �������� �������������� ������������ �������� (��)
void Brain::Core::S1(int %ix, int %iy) {
	// ������� ����� ����������� �������� (������������� ����������� �������)
	ID S = ID();

	S.ConsStatus = 0; // 0 - �� ��������������
	S.NAct = 1;
	S.TCreat = this->CT;
	S.P = gcnew array<unsigned short, 2>(this->R_ID * 2 + 1, this->R_ID * 2 + 1); // ������, ����������� ���������� ������� (�������)

	for (int ix1 = ix - this->R_ID; ix1 <= ix + this->R_ID; ix1++) {
		for (int iy1 = iy - this->R_ID; iy1 <= iy + this->R_ID; iy1++) {
			// �������� ���������������� � ���������
			/*
			S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)] = this->Cortex[ix1, iy1].AAccumExtraSinapse *  // ����������� �� ����� �������� ���������������� ���������� ����������
																	this->SensTable[this->Cortex[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)].Type, this->Cortex[ix, iy].Type, 0]; // ������� �.�.: ������� ������ ���������������� �������� (0)
			*/
			// ������� �.�.: ��� �������, ��� ����� ������ ���� ���:
			S.P[ix1 - (ix - this->R_ID), iy1 - (iy - this->R_ID)] = (unsigned short)( this->Cortex[ix1, iy1].AAccumExtraSinapse * // ����������� �� ����� �������� ���������������� ���������� ����������
																					  this->SensTable[this->Cortex[ix1, iy1].Type, this->Cortex[ix, iy].Type, 0] ); // ������� �.�.: ������� ������ ���������������� �������� (0)
		}
	}

	// ��������� � ������ �������� ��������� ���������� ��������� ���� �� (��� ��������)
	if (this->CheckedPatterns->Count == 1) {
		S.NP = CheckedPatterns[0];
	}
	else {
		S.NP = 0;
	}

	this->Cortex[ix, iy].ID->Add(S);
} // �������� �������������� ������������ �������� (��). S1(int %ix, int %iy)