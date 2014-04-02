#include "stdafx.h"

#include "Brain.h"



//
// ����������� ����
//
Brain::Core::Core() {
	// ���������� ���������� ��������� ����� �� 0 �� 1
	this->rnd = gcnew System::Random();
	
	this->initCore();
	
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


void Brain::Core::initCore() {
	//
	// ������������� �����:
	//
	
	// ���-�� ����� �������� (int)
	N_Types = 2;
	// ������� ���������������� ������������� � ���������������� ���������� � ����������, ������������ �������� �������� ������� ����
	SensTable = gcnew array<unsigned short, 3>(N_Types, N_Types, 2);
	// ����������� (������� �.�.):
	// (N_Types, N_Types, 2);
	// 2 - ��������, ��� ����� ��� ���� ����������: ������������� � ����������������
	// ��� ����� �� ���������, ���� ���� ����� �������� ������ ������

	// ������� ���� (int)
	X_C = 150;
	Y_C = 150;

	// �������� ��������: ����
	pic = gcnew System::Drawing::Bitmap(X_C, Y_C);
	Gpic = System::Drawing::Graphics::FromImage(this->pic);
		
	// ����� ���������� ��������� (int)
	NP = 5;
	// ����������� (������� �.�.):
	// ���� ���� � ��������� ��������� ����������, ������� ������������ �������� ������� �����

	// ����� ��������� ��������� ���������� // ������� �.�.: �.�. ������� �������
	PatSet = gcnew array<Pattern>(NP);

	// ������ �������� ��������� ���������� (int)
	R_Pat = 6;

	// ���-�� �������� � ������� ��������� ���������� (int)
	NNPat = 15;

	// ����
	Cortex = gcnew array<Neuron, 2>(X_C, Y_C);

	// ������ ���������� �������� � ������������� ��������� ��� ������� �� �����. ���� ����� ������� ����������� ���������� ����������
	LImin = gcnew array<double>(N_Types);
	// ������� ��������� ������� ��������� ������������� �������� ��� ���������� ���������� ����������
	LImin[0] = 0.01;
	LImin[1] = 0.01;

	// ���������� �������� �� ����������� ��� ID (����� � ������ �� ����������� � ���������) (int)
	// ������� �.�.: ��� ������������� ����������
	R_ID = 15;
	// ���������� �������� �� ����������� ��� ����� W (����� � ������ �� ����������� � ���������) (int)
	// ������� �.�.: ��� ������������� ����������
	R_W = R_ID;
	// ������ ��������������� ����������. ������ � �������� ���������� �� �������� (������� ���������) (int)
	RLAM = R_W;
	// ������ ������������� ���������� (int)
	NLocalAM = 2 * RLAM + 1;
	LocalAM = gcnew array<float, 2>(NLocalAM, NLocalAM);
	// �������� ��������������� ����������. ���������
	Sigma2LAM = (float)System::Math::Pow(5., 2);

	// ������� ���� (int)
	CT = 0;

	// ������ ���������� �������������� ���������� (������) (int)
	TAhalf = 4;
	// ������� ���������� �� ���� ����
	KA = (float)( 1 / System::Math::Pow( 2.0, 1.0 / TAhalf ) );
	
	// ���� ����������� ��������� ���������� ����
	Field_A_Accum = gcnew array<float, 2>(X_C, Y_C);
	// ����� ���������� ��������� ���������� ��� ����������� ��������
	L_ActL1 = 0.15f;
	
	// ���� ���������� ���������� ����
	Field_A = gcnew array<float, 2>(X_C, Y_C);
	// ����� ���������� �� �������� ��� ����������� ��������
	// ������� �.�.: ����� ������, ����� ����� 2% �������� �������������� ��� ����������� ��������
	L_ActLS = (float)( 0.02 * System::Math::Pow( (float)(2 * R_W + 1), 2 ) );

	// ���� ��������� ���������� ����
	Field_A_Evoced = gcnew array<float, 2>(X_C, Y_C);
	// ���� �������� ���������� ����
	Field_A_Wave = gcnew array<float, 2>(X_C, Y_C);
	// ���� ��������� ������������� �������� ������� �� �����
	Field_A_Relax = gcnew array<float, 3>(X_C, Y_C, N_Types);

	// ����������������� ������ ����������������� ���������� (int)
	T_ID = 10;
	// ����������������� ������ (int)
	T_Passive = T_ID * 4;
	// ����� ������ ���������� ���������� ��� ������������� �������� (int)
	T_SinapseLearn = T_Passive;
	// ����� ����� ��������� ��������� ��������
	CortexA = gcnew array<unsigned short, 3>(X_C, Y_C, T_SinapseLearn);
	// ����� ������ ���������� ���������� ��� ���������������� �������� (int)
	T_ExtraSinapseLearn = 4;

	// ����������� ���������� ��� ������ �������
	LAmin = 0.008;
	// ������ ���������� ��� ��������� ���������� �������
	LAmax = 0.025;

	// ����� ��������� �� ��������������
	L_act = 0.7;
	
	// ���������� ����, ����������� ��� ������������ (int)
	NCons = 5;

	// ������� �.�.: ��������� ��������� ��� ������������� ��������� ��������� ����������
	CheckedPatterns = gcnew System::Collections::ObjectModel::Collection<int>();

	// ����������� ������� ����������� ������
	Psp = 0.02;
	
	// ������� ���� (int)
	CurLayer = 0;

	// ��������� ����������� ���������� (int)
	NSer = 300;
	CurPosA = 0;
	AWave = gcnew array<double>(NSer);

	// ���������� ���� ���������� ������� (int)
	TX1 = X_C / 4;
	TY1 = Y_C / 4;
	// ���������� ���� ��������� ������� (int)
	TX2 = X_C / 2;
	TY2 = Y_C / 2;
	// ������� ������� (int)
	TD = 30;

	// ��������� ��� ������������ ������� ����� ���������
	FPic = false; // ������� �.�.: ���� ����, ���� ��� �� ���� ������� ����� �������� ���������� �� �������
	// (int)
	NPic = 16; // ������� �.�.: ���������� ��������
	WPic = 4; // ������� �.�.: ���������� �������� �� �����������
	Bigpic = gcnew System::Drawing::Bitmap(X_C * WPic, Y_C * 4);

	// ������� ������������������ �����
	LayersSequence = gcnew array<int>(T_SinapseLearn);

	// ���� ��������� �������� ��������-����������
	FLearn = true;

	// ���� �������� ��������
	// ������� �.�.: ������, ������ ������ ������ �� ����� �������� (�� ���������� <= R_W) ���������, � ��� �� ����
	CortexW = gcnew array<float, 4>(X_C, Y_C, R_W * 2 + 1, R_W * 2 + 1);

	// ����� ��������� �������
	L_Spike = 0.6f;

	// ����������������� ��������� ����������
	T_EnvokedA = (int)(T_Passive * 1.5);

	// ������� ��������� ��������
	pic1 = gcnew System::Drawing::Bitmap(X_C, Y_C);
	
	// ������������ ���������������� ���������������
	// ����� �������� �� �����������
	L_GameLive_Low = 0.15;
	// ����� �������� �� �������������
	L_GameLive_high = 0.25;

	// ������� ��������� ����������
	picEvokedA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// ������� ����������� ����������
	picActAccum = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// ������� ���� ���������� (��������)
	picFieldA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// ������� ����������
	picRelaxA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	// ������� ���� ����� (��������)
	picWaveA = gcnew System::Drawing::Bitmap(X_C, Y_C);
	
	// ������� ���� ����������, ������������ �� �������� (�� ������������)
	// picAAvg = gcnew System::Drawing::Bitmap(X_C, Y_C);

	//-----------------------------------------------------------------------------------------------
	// ������� �.�.: ��� ���� �� ������������:
	/*
	// ������� ���� ���������� ����������
	SReceptF = (int)System::Math::Pow(RLAM * 2 + 1, 2);
	// ���������� �������� ��������� � ������ ����� (int)
	NPC = 1;
	// ������� (int)
	N_Neurotransmitters = 2;
	// �����, ���������� �� ������������ (int)
	T_L1 = 50;
	*/
}


// ������� ������� ��������������� ����������
void Brain::Core::SetSens() {
	// �������� ������� (��� 0):
	// �������� ������ - A
	// ���������������� ��������� - A
	// ������������� ��������� - B

	// ������� ��������� (��� 1):
	// �������� ������ - A, B
	// ���������������� ��������� - ---
	// ������������� ��������� - A

	// ������ �������
	// ��� ����������� �������, ��� ������������, ���������������� ���������������� (0) - ������������� (1)
	this->SensTable[0, 0, 0] = 1;
	this->SensTable[0, 0, 1] = 0;

	this->SensTable[0, 1, 0] = 0;
	this->SensTable[0, 1, 1] = 1;

	this->SensTable[1, 0, 0] = 1;
	this->SensTable[1, 0, 1] = 1;

	this->SensTable[1, 1, 0] = 0;
	// this->SensTable[1, 1, 1] = 0; // ����������� (������� �.�.): ���� ������� �� ������������� �������� ����
	this->SensTable[1, 1, 1] = 1; // ����������� (������� �.�.): � ���� ������� �������������

	// ����������� (������� �.�.):
	//
	// ������ �������
	// ��� ����������� �������, ��� ������������, ��� ���������: ���������������� (0) ��� ������������� (1)
	//
	// ������ ��� ������, ����� �������� ���������:
	// this->SensTable[��� ����������� �������, ��� ������������, ��� ���������] = 1;
	//
	// ������ ��� ������, ����� �������� �� ���������:
	// this->SensTable[��� ����������� �������, ��� ������������, ��� ���������] = 0;
} // ������� ������� ��������������� ����������. SetSens()


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


// ������ ���������� �����
void Brain::Core::DrawChart() {
	int YSize = 40;
	int Y = 0;
	double Amax = 0.05;
	pic2 = gcnew System::Drawing::Bitmap(this->NSer, YSize);

	int i = 0;
	for (int k = this->CurPosA + 1; k < this->NSer; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}

	for (int k = 0; k <= this->CurPosA; k++) {
		Y = (int)( this->AWave[k] * (YSize - 1) / Amax );
		if (Y >= YSize - 1) {
			Y = YSize - 1;
		}
		pic2->SetPixel(i, YSize - Y - 1, System::Drawing::Color::Black);
		i++;
	}
} // ������ ���������� �����. DrawChart()


// ������� ������ ��������� ���� (������ ��������)
void Brain::Core::Draw_Status() {
	System::Drawing::Pen ^Pen;
	int i1;
	int i2;
	
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			switch (this->Cortex[i, j].Status) {
				case 0: // ����� // 0 - ������� // ����������� (������� �.�.): ������
					this->pic->SetPixel(i, j, System::Drawing::Color::Black);
					break;
				case 1: // ��������� ���������� // 1 - ��������� ���������� // ����������� (������� �.�.): �������
					this->pic->SetPixel(i, j, System::Drawing::Color::Orange);
					break;
				case 2: // ������ ����� �������������� // 2 - �������� ���������� // ����������� (������� �.�.): �����
					this->pic->SetPixel(i, j, System::Drawing::Color::Gray);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 3: // ����� ����� �������������� // 3 - ������ ���� �������� ����������
					this->pic->SetPixel(i, j, System::Drawing::Color::White);
					if (this->Cortex[i, j].ERR) {
						this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					}
					break;
				case 4: // ������� � ������������� ��������� // 4 - ���������� ����� ��������� ������ // ����������� (������� �.�.): �����
					this->pic->SetPixel(i, j, System::Drawing::Color::DarkBlue);
					break;
				case 5: // ������� ���������� ����� // 5 - ���������� ���������� // ����������� (������� �.�.): �������
					this->pic->SetPixel(i, j, System::Drawing::Color::Green);
					break;
				case 6: // ������������ ���������� // 6 - ������� �����
					this->pic->SetPixel(i, j, System::Drawing::Color::Red);
					break;
			}
		}
	}

	// ���� ���� �������� �������, �� ��������� ��� ������
	// if ((this->Text == "Form1" && cortex2 != nullptr) || this->Text == "2")
	// ������� �.�.: ����� ����� ���:
	if ( (this->cortex2 != nullptr) != (this->cortex0 != nullptr) ) {
		Pen = gcnew System::Drawing::Pen(System::Drawing::Color::White, 1);
		
		if ( this->cortex2 != nullptr ) {
			i1 = this->TX1;
			i2 = this->TY1;
		}
		else
		{
			i1 = this->TX2;
			i2 = this->TY2;
		}
		
		this->Gpic->DrawLine(Pen, i1, i2, i1 + TD, i2);
		this->Gpic->DrawLine(Pen, i1, i2 + TD, i1 + TD, i2 + TD);
		this->Gpic->DrawLine(Pen, i1, i2, i1, i2 + TD);
		this->Gpic->DrawLine(Pen, i1 + TD, i2, i1 + TD, i2 + TD);
	}
} // ������� ������ ��������� ���� (������ ��������). Draw_Status()


// ������������ ����� ��������
void Brain::Core::addpic() {
	int s = this->CT - this->PicStart - 1; // ������� �.�.: ����� �������� � �����, ������ � ���� � ������ (NPic - 1)
	int Y = (int)System::Math::Floor( s / (double)this->WPic ); // ������� �.�.: ����� ������, ������� � 0
	int X = s - Y * this->WPic; // ������� �.�.: ����� �������, ������� � 0
	X = X * X_C; // ������� �.�.: ���������� ������� ������ ���� ��������� �������� � �����
	Y = Y * Y_C; //               �� ������� ������� ��������

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			this->Bigpic->SetPixel(X + ix, Y + iy, this->pic->GetPixel(ix, iy));
		}
	}

	if (s == this->NPic - 1) { // ������� �.�.: ��������� �� ��������� � ����� (������)
		this->FPic = false;  // ������� �.�.: �������� ������������ ����� ��������
	}
} // ������������ ����� ��������. addpic()


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


// �������� ������������� �����. ������� �����������
void Brain::Core::LearnW1() {
	for (int ix = this->R_W; ix < this->X_C - this->R_W; ix++) {
		for (int iy = this->R_W; iy < this->Y_C - this->R_W; iy++) {
			if (this->Cortex[ix, iy].Status == 2 && // 2 - �������� ���������� // ����������� (������� �.�.): �����
				this->Field_A_Accum[ix, iy] > this->L_ActL1 && // �����, ����� ���� ����������� (����������) ��������� ���������� ���� ��������� ����� L_ActL1
				this->Field_A[ix, iy] > this->L_ActLS && // �����, ����� ���� ���������� ���������� ���� ��������� ����� L_ActLS
				this->Cortex[ix, iy].StatusL == 0) { // 0 - ��������
					
					this->Cortex[ix, iy].StatusL = 1; // 1 - �����������
					this->Cortex[ix, iy].TL = this->CT; // TL - ��������� ������� ��������

					// �������� ��������� � ��������
					for (int ix1 = 0; ix1 <= 2 * this->R_W; ix1++) {
						for (int iy1 = 0; iy1 <= 2 * this->R_W; iy1++) {
							this->CortexW[ix, iy, ix1, iy1] = Cortex[ix + ix1 - this->R_W, iy + iy1 - this->R_W].AAccumSinapse; //* SensTable(Cortex(ix + ix1 - R_W, iy + iy1 - R_W).Type, Cortex(ix, iy).Type, 1)
						}
					}
			}
		}
	}
} // �������� ������������� �����. ������� �����������. LearnW1()


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


// �������� �� ��������� �������
void Brain::Core::TMove() {
	if (this->cortex2 == nullptr) { // ������ �� ������, ���� ������� �� ������
		return;
	}

	for (int ix = 0; ix < this->TD; ix++) { // TD - ������� �������
		for (int iy = 0; iy < this->TD; iy++) {
			// ���������� ���� ���������� ������� TX1, TY1
			// ���������� ���� ��������� ������� TX2, TY2
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].Status = this->Cortex[ix + this->TX1, iy + this->TY1].Status;
			this->cortex2->CortexA[ix + this->TX2, iy + this->TY2, this->cortex2->CurLayer] = this->CortexA[ix + this->TX1, iy + this->TY1, this->CurLayer];
			this->cortex2->Cortex[ix + this->TX2, iy + this->TY2].T = this->Cortex[ix + this->TX1, iy + this->TY1].T;
		}
	}
	
	this->cortex2->Wave();
} // �������� �� ��������� �������. TMove()


// ������� ��������� ����������
void Brain::Core::Draw_EvokedA() {
	int Y; // ���� � R=G=B
			
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Evoced[ix, iy] / 0.08 * 255 ); // Field_A_Evoced - ���� ��������� ���������� ����
			if (Y > 255) {
				Y = 255;
			}
			this->picEvokedA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ��������� ����������. Draw_EvokedA()


// ������� ����������� ����������
void Brain::Core::Draw_ActAccum() {
	int Y; // ���� � R=G=B
	
	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Accum[ix, iy] * 500 ); // Field_A_Accum - ���� ����������� ��������� ���������� ����
			if (Y > 255) {
				Y = 255;
			}
			this->picActAccum->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ����������� ����������. Draw_ActAccum()


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


// ������� ����������
void Brain::Core::Draw_RelaxA(int ntype) {
	int Y; // ���� � R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Relax[ix, iy, ntype] / 0.04 * 255 ); // Field_A_Relax - ���� ��������� ������������� �������� ������� �� �����
			if (Y > 255) {
				Y = 255;
			}
			this->picRelaxA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ����������. Draw_RelaxA(int ntype)


// ������� ���� ����� (��������)
void Brain::Core::Draw_WaveA() {
	int Y; // ���� � R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = (int)( this->Field_A_Wave[ix, iy] / 0.04 * 255 ); // Field_A_Wave - ���� �������� ���������� ����
			if (Y > 255) {
				Y = 255;
			}
			this->picWaveA->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ���� ����� (��������). Draw_WaveA()


// ��������� ������� ��������� (��� ��������� ������� ��������� ��������� ����������)
// ������� �.�.: ����� ��������� ������ �������� ��������� ���������� � ���������������� ��������� ��������
void Brain::Core::PatternsActivation() {
	for (int i = 0; i < this->X_C; i++) {
		for (int j = 0; j < this->Y_C; j++) {
			this->Cortex[i, j].Status = 0; // 0 - ������� // ����������� (������� �.�.): ������
			this->CortexA[i, j, this->CurLayer] = 0; // ����� ����� ��������� ��������� ��������
		}
	}
	
	this->ActivePatternsRefresh(); // ���������� ������� �������� ������� �������
} // ��������� ������� ��������� (��� ��������� �� �������� ��������� ��������� ����������). PatternsActivation()


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


//-----------------------------------------------------------------------------------------------
// ������� ���� ����������, ������������ �� �������� (���� ����� �� ������������)
/*
void Brain::Core::MakePicAAvg() {
	int Y; // ���� � R=G=B

	for (int ix = 0; ix < this->X_C; ix++) {
		for (int iy = 0; iy < this->Y_C; iy++) {
			Y = this->Cortex[ix, iy].AAccumSinapse * 255; //AAccumSinapse - ����������� �� ����� �������� �������� ����������
			this->picAAvg->SetPixel(ix, iy, System::Drawing::Color::FromArgb(Y, Y, Y));
		}
	}
} // ������� ���� ����������, ������������ �� �������� (���� ����� �� ������������). MakePicAAvg()
*/


// ��������� ������� ������� �������
void Brain::Core::Draw_Numbers() {
	System::Drawing::SolidBrush ^brush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Yellow);
	System::Drawing::Font ^font = gcnew System::Drawing::Font("Arial", 10, System::Drawing::FontStyle::Bold);

	for (int i = 0; i < this->NP; i++) {
		this->Gpic->DrawString(i.ToString(), font, brush, (float)this->PatSet[i].ix, (float)this->PatSet[i].iy);
	}
} // ��������� ������� ������� �������. Draw_Numbers()