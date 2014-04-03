#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"





// �������� ��������� ��������� ��������� ����������
// ������� �.�.: ������ ��� ����� ��� �� ������������, � ������ ���������� ��� ����������� ������� ������
void Brain::Core::PatCreate() {
	//
	// ����������� (������� �.�.):
	//
	// �������� ���������� ���������� ������� � �������� ��������� ����������
	double Ang = 0;
	double R = 0;
	//
	// ����������� (������� �.�.):
	//
	// ��������� ���������� ������� � �������� ��������� ���������� ������������ ������ ��������
	// � ���������� ������� ���������
	int dx = 0;
	int dy = 0;

	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			this->pic->SetPixel(i, j, System::Drawing::Color::Black);
		}
	}

	for (int i = 0; i < this->NP; i++) {
		//
		// ����������� (������� �.�.):
		//
		//this->PatSet[i].ix = (this->X_C - 3 * this->R_Pat) * rnd.NextDouble() + this->R_Pat;
		//this->PatSet[i].iy = (this->Y_C - 3 * this->R_Pat) * rnd.NextDouble() + this->R_Pat;
		this->PatSet[i].ix = (int)((this->X_C - 6 * this->R_Pat) * this->rnd->NextDouble() + 3 * this->R_Pat);
		this->PatSet[i].iy = (int)((this->Y_C - 6 * this->R_Pat) * this->rnd->NextDouble() + 3 * this->R_Pat);
		
		this->PatSet[i].P = gcnew array<unsigned short, 2>(this->R_Pat * 2 + 1, this->R_Pat * 2 + 1);

		for (int j = 0; j < this->NNPat; j++) {
			Ang = 2 * System::Math::PI * this->rnd->NextDouble();
			R = this->R_Pat * this->rnd->NextDouble();

			// dx = R * System::Math::Sin(Ang);
			// dy = R * System::Math::Cos(Ang);
			//
			// ����������� (������� �.�.):
			//
			// ��� ����������, ��� ��� ����� ��������� � ����� ������ ����������,
			// ���� ��� � �� ����� �������� (sin � cos ������� �������):
			dx = (int)( R * System::Math::Cos(Ang) );
			dy = (int)( R * System::Math::Sin(Ang) );

			this->PatSet[i].P[this->R_Pat + dx, this->R_Pat + dy] = 1;

			// ���������� ������� ������� �������
			this->Cortex[this->PatSet[i].ix + dx, this->PatSet[i].iy + dy].StatusL = 5;
			//
			// ����������� (������� �.�.):
			//
			// 5 - �� �������� �������� (������� �����)
		}

		// ������� �.�.: ��� NP ��������� ��������� ���������� �������� ������ ���������
		/*
		for (int ix = 0; ix <= 2 * this->R_Pat; ix++) {
			for (int iy = 0; iy <= 2 * this->R_Pat; iy++) {
				if (this->PatSet[i].P[ix, iy] == 1) {
					this->pic->SetPixel(this->PatSet[i].ix - this->R_Pat + ix, this->PatSet[i].iy - this->R_Pat + iy, System::Drawing::Color::White);
				}
			}
		}
		*/
		// ������� �.�.: ����� �� ������� �������� ��� �������� ������ � ���� �� ������� �������,
		// ����� ����� �������� ��������� ��������� ��������� ���������� ������ �������
		// PatternsActivation(), � ������� ���������� Draw_Status(). ����� �������� �� �����
	}
} // �������� ��������� ��������� ��������� ����������. PatCreate()