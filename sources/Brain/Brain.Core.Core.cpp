#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"



//
// ����������� ����
//
Brain::Core::Core() {
	// ���������� ���������� ��������� ����� �� 0 �� 1
	this->rnd = gcnew System::Random();
	
	this->InitCore();
	
	this->SetSens();
	
	this->PatCreate();
	
	// ��������� �������� ����
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Cortex[ix, iy].Type = 0;
			// ����������� (������� �.�.): 0 - ������� �����
			// ���������� ��� ������ �������
			this->Cortex[ix, iy].ID = gcnew System::Collections::ObjectModel::Collection<ID>();
		}
	}

	// �������� ������� �������������� ����������
	// ����� ���� ����� ���� �������. ��� ��������� ������������� ������ ��� ����� � ��������� ��������������� ��������
	// ����������� (������� �.�.):
	// ����� ���� � ����� �������� ������ �� �������� ����� �����, � ������:
	// ����������� �������, �� ������� ������ ������ ��� ������������
	float dx = 0;
	float dy = 0;
	for (int ix = 0; ix < this->NLocalAM; ix++) {
		for (int iy = 0; iy < this->NLocalAM; iy++) {
			dx = (float)(ix - this->RLAM);
			dy = (float)(iy - this->RLAM);
			this->LocalAM[ix, iy] = (float)( (1 / (2 * System::Math::PI * this->Sigma2LAM)) * System::Math::Exp( -(System::Math::Pow(dx, 2) + System::Math::Pow(dy, 2)) / (2 * this->Sigma2LAM) ) );
		}
	}
}