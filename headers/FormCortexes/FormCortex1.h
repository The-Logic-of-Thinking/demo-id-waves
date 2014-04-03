#include "../Brain/Brain.h"
#include "FormCortex2.h"


namespace IdWaves {
	public ref class FormCortex1 : public System::Windows::Forms::Form
	{
		public:
			FormCortex1(void);
			
		protected:
			~FormCortex1();
					
		private:
			System::Windows::Forms::Button^  Button1;
			System::Windows::Forms::Button^  Button11;
			System::Windows::Forms::Button^  Button5;
			System::Windows::Forms::Label^  Label1;
			System::Windows::Forms::Button^  Button3;
			System::Windows::Forms::CheckedListBox^  CheckedListBox1;
			System::Windows::Forms::Button^  Button6;
			System::Windows::Forms::Label^  Label3;
			System::Windows::Forms::Label^  Label2;
			System::Windows::Forms::Button^  Button2;
			System::Windows::Forms::Button^  Button4;
			System::Windows::Forms::ComboBox^  ComboBox2;
			System::Windows::Forms::PictureBox^  PictureBox4;
			System::Windows::Forms::PictureBox^  PictureBox2;
			System::Windows::Forms::ComboBox^  ComboBox1;
			System::Windows::Forms::PictureBox^  PictureBox3;
			System::Windows::Forms::PictureBox^  PictureBox1;
			System::Windows::Forms::Label^  label4;
			System::ComponentModel::Container ^components;

			void InitializeComponent(void);
			
			// ���������� ����
			Brain::Core ^bcore;

			// ������� �.�.: �� ������, ���� ����� �������� �������
			IdWaves::FormCortex2 ^formcortex2;
			
			// ���������� ��������� �� ������� �� ����� ����������
			void block ();
			
			// ������������� ��������� ����� ���������� ��������
			void unblock ();

			// �������������
			void FormCortex1_Load(Object ^sender, System::EventArgs ^e);
			
			// ������ "������ � ������"
			void Button1_Click(Object ^sender, System::EventArgs ^e);
			
			// ����������� �������� ��������
			void DrawLearningPicture();
			
			// ������� �.�.: ���� �������� ���, ��� ������� � �������� � ������������ ����������� ������ �����
			void Tact();
			
			// ������ "1 ����"
			// ����������� (������� �.�.):
			// ������ ���� ��������� ����� (������ ����� �����)
			void Button3_Click(Object ^sender, System::EventArgs ^e);
			
			// 50 ������ �����
			void Button11_Click(Object ^sender, System::EventArgs ^e);
			
			// 200 ������ �����
			void Button5_Click(Object ^sender, System::EventArgs ^e);
			
			// ���. ����� ��������
			void Button2_Click(Object ^sender, System::EventArgs ^e);
			
			// ������� �.�.: ���������� �� ������� ����
			// ����������� ���������� ���� ����������
			void DrawSomeActivity();
			
			// ��������/�������� ��������� �������
			void Button4_Click(Object ^sender, System::EventArgs ^e);
			
			// ���/���� ����������� �������� ��������
			void Button6_Click(Object ^sender, System::EventArgs ^e);
		
			// ������� �.�.: ����� ��������� ��������� ���������� ��� �������������
			void CheckedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

			// ����������� ��� ���������� ������ ����
			void CheckedListBox1_MouseUp(Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
	};
}