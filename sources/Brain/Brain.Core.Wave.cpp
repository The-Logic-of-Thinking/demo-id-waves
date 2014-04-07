#include "../../headers/stdafx.h"

#include "../../headers/Brain/Brain.h"




// ������� ���� ���� �����
void Brain::Core::Wave() {
	double AW;
	int k;
	int sum;

	// ������ ����� ��������� ����
	this->CalcFields();

	/*
	// ����������� (������� �.�.): ���� ���� ��������� � ��������� ������� � ������� (������) �����
	// ����������� ���������� ���� ����������
	switch (ComboBox1->SelectedIndex) {
		case 0:
			Draw_ActAccum(); // ���. ����������� (��������)
			break;
		case 1:
			Draw_FieldA(); // ���. ������������ (��������)
			break;
		case 2:
			Draw_EvokedA(); // ���. ���������
			break;
		case 3:
			Draw_WaveA(); // ���. ��������
			break;
		case 4:
			Draw_RelaxA(); // ������� ����������
			break;
	}
	*/

	// ���������� �������� ��������� ��������
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 0: // ����� // 0 - ������� // ����������� (������� �.�.): ������
					// ���������� ��������� ��������������, �� �� ������������ CortexA, � ������ ������ ������
					this->S0(ix, iy);
					
					// ���� ����������� ���
					if (this->Cortex[ix, iy].Status == 0) {
						// ������� ���������� ���������� �����, �� ��� �� ������ �������� ������, 
						if (this->Field_A[ix, iy] > this->LAmin && // ���� ���������� ���������� ����
							this->Field_A_Wave[ix, iy] < this->LAmax && // ���� �������� ���������� ����
							this->Field_A_Relax[ix, iy, this->Cortex[ix, iy].Type] < this->LImin[this->Cortex[ix, iy].Type]) { // ���� ��������� ������������� �������� ������� �� �����
							// � ������������� �������� ������ ����, ��
							if (this->rnd->NextDouble() < this->Psp) {
								// ������� ��������� �� ���������� � �������������
								this->Cortex[ix, iy].Status = 7; // 7 - ��������� �������� �� ��������� ������� � �������� ��������������
								this->Cortex[ix, iy].T = this->CT + this->T_ExtraSinapseLearn;
							}
						}
					}
					break;
				case 7: // 7 - ��������� �������� �� ��������� ������� � �������� ��������������
					// ���������� ��������� ��������������, �� �� ������������ CortexA, � ������ ������ ������
					this->S0(ix, iy);
					// ������� �.�.: ���� ������ �� ���������
					if (this->Cortex[ix, iy].Status == 7) {
						if (this->Field_A_Wave[ix, iy] > this->LAmax) {// ���� �������� ���������� ����
							// ���������� ��������� ���������, ���� ���������� ��������� �����
							this->Cortex[ix, iy].Status = 0;
						}
						else if (this->CT > this->Cortex[ix, iy].T) {
							// �������� ���������� ���������� � �������� ID �������� 
							this->Cortex[ix, iy].Status = 5; // 5 - ���������� ���������� // ����������� (������� �.�.): �������
							this->Cortex[ix, iy].T = this->CT;
							// �������� �������������� ������������ ��������
							this->S1(ix, iy);
						}
					}
					break;
				case 1: // ��������� ���������� // 1 - ��������� ���������� // ����������� (������� �.�.): �������
					// ���������� ��������� ���������� �� ��������� ������
					if (this->CT > this->Cortex[ix, iy].T) { // ������� �.�.: ���� ������ ��� ���������, �� ������ ����� ������� � ��������� �����
						this->Cortex[ix, iy].Status = 0;
					}
					break;
				case 2: // ����� �������� ���������� // 2 - �������� ���������� // ����������� (������� �.�.): �����
					// ���� ����� �����������, �� ��������� � ��������� ������������������
					if (this->CT > this->Cortex[ix, iy].T) {
						// ������ ����� ����������							 // ��� ������, ��� ������� ����� ��������� ����������
						this->Cortex[ix, iy].T = this->CT + this->T_Passive; // * (1 + Field_A_Accum(ix, iy) / 0.02)
						this->Cortex[ix, iy].Status = 4; // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
						// ������� �.�.: ��� ������ � ������ �� �������
						this->Cortex[ix, iy].ERR = false;
					}
					break;
				case 3: // ������ ���� �������� ���������� // 3 - ������ ���� �������� ����������
					this->Cortex[ix, iy].Status = 2; // ����� �������� ���������� // 2 - �������� ���������� // ����������� (������� �.�.): �����
					break;
				case 4: // ��������� ������������������ // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
					if (this->CT > this->Cortex[ix, iy].T) { // ������� �.�.: ���� ���������� ������, ��
						this->Cortex[ix, iy].Status = 0;     // ��������� � ��������� �����
					}
					break;
				case 5: // ���������� ����� // 5 - ���������� ���������� // ����������� (������� �.�.): �������
					this->Cortex[ix, iy].Status = 2; // ����� �������� ���������� // 2 - �������� ���������� // ����������� (������� �.�.): �����
					break;
				//case 6: break; // ������������ (�������) ���������� // 6 - ������� �����
			}
		}
	}

	// ������� ������� ���������� ������ ���������� ����, �������������� ��������
	AW = 0; // ��� double
	// �������� ������ ��������� ����
	this->CurLayer += 1;
	if (this->CurLayer == this->T_SinapseLearn) {
		this->CurLayer = 0;
	}
	
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			switch (this->Cortex[ix, iy].Status) {
				case 1: // 1 - ��������� ���������� // ����������� (������� �.�.): �������
				case 2: // 2 - �������� ���������� // ����������� (������� �.�.): �����
				case 3: // 3 - ������ ���� �������� ����������
				case 5: // 5 - ���������� ���������� // ����������� (������� �.�.): �������
				case 6: // 6 - ������� �����
					this->CortexA[ix, iy, this->CurLayer] = 1; // ����� ����� ��������� ��������� ��������.
					AW += 1;
					break;
				default: // 0 - ������� // ����������� (������� �.�.): ������
						 // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
						 // 7 - ��������� �������� �� ��������� ������� � �������� ��������������
					this->CortexA[ix, iy, this->CurLayer] = 0; // ����� ����� ��������� ��������� ��������.
					break;
			}
		}
	}

	// ������ �����
	this->CurPosA += 1;
	if (this->CurPosA == this->NSer) {
		this->CurPosA = 0;
	}
	// ��������� ����������� ����������
	this->AWave[this->CurPosA] = AW / ((this->X_C - 2 * this->R_ID) * (this->Y_C - 2 * this->R_ID));
	
	// ������ ���������� �����
	this->DrawChart();
	// ��������� �� ������������ ��� �������� ����������������� ������
	// ������� �.�.: ������������ �������� ��������
	this->Draw_Status();

	this->CT += 1;

	if (this->FPic) {
		this->addpic();
	}

	// ��������� ������������������ ����� (0 - �������, 1 - ���������� � �.�.)
	k = this->CurLayer; // ��� int
	for (int i = 0; i < this->T_SinapseLearn; i++) {
		this->LayersSequence[i] = k;
		k -= 1;
		if (k < 0) {
			k = T_SinapseLearn - 1;
		}
	}

	// ������ ����������� �� ������� ����������
	for (int ix = this->R_ID; ix < this->X_C - this->R_ID; ix++) {
		for (int iy = this->R_ID; iy < this->Y_C - this->R_ID; iy++) {
			sum = 0; // ��� int
			
			// ������� �.�.: ������, ��� AAccumExtraSinapse � AAccumSinapse - ������ ����� ����, ���
			// ��������� T_ExtraSinapseLearn � T_SinapseLearn (T_SinapseLearn > T_ExtraSinapseLearn)
			// ������, ��������������, ������ ��������� ���� � �����-�� ���� ����������.
			//
			// ������� �.�.: � ���������� ������-�� ��� ����� ���������� ������������� �������,
			// � �� Byte, ��� �� ������ ������ ��� ������. ������?
			
			/*
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++)
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			if (sum > 0)
				this->Cortex[ix, iy].AAccumSinapse = 1;
			else
				this->Cortex[ix, iy].AAccumSinapse = 0;
			*/

			// ������� �.�.: ��� ���� ��������� �� ����������, ����� �������� ���:
			this->Cortex[ix, iy].AAccumExtraSinapse = 0;
			this->Cortex[ix, iy].AAccumSinapse = 0;
			
			for (int i = 0; i < this->T_ExtraSinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumExtraSinapse = 1;
				this->Cortex[ix, iy].AAccumSinapse = 1;
				continue;
			}									
			
			for (int i = this->T_ExtraSinapseLearn; i < this->T_SinapseLearn; i++) {
				sum += this->CortexA[ix, iy, this->LayersSequence[i]];
			}
			if (sum > 0) {
				this->Cortex[ix, iy].AAccumSinapse = 1;
			}
		}
	}
	
	// ��������
	if (this->FLearn) {
		// ������ ��������� ����������
		this->Calc_Evoked_Activity();

		//this->MakePicFieldA();

		// �������� ������������� �����. ������� �����������
		this->LearnW1();
		
		// ������ �����������, ������ �� ������� ���������
		// this->LearnW2();
	}

	// �������� �� ��������� �������
	this->TMove();
} // ������� ���� ���� �����. Wave()