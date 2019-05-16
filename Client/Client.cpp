// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void wait(int tg)
{
	clock_t endwait;
	endwait = clock() + tg*CLOCKS_PER_SEC;
	while (clock() < endwait);

}
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			CSocket client;
			char sAdd[1000];
			unsigned int port = 1234; //Cung port voi server
			AfxSocketInit(NULL);

			//1. Tao socket
			client.Create();

			// Nhap dic chi IP cua server
			cout << "\n Nhap dia chi IP cua server: ";
			cin >> sAdd;

			int lengten;
			int id;
			char *temp;
			if (client.Connect(CA2W(sAdd), port))
			{
				printf("\n Client da ket noi toi server");
				//Nhan tu server, cho biet day la client thu may

				client.Receive((char*)&id, sizeof(id), 0);
				printf(" \nDay la client thu %d\n", id + 1);

				//nhan thong tin tu user nhan do dai 
				client.Receive((char*)&lengten, sizeof(int), 0);
				temp = new char[lengten + 1];
				//nhan chuoi
				client.Receive((char*)temp, lengten, 0);
				//in ra thong deip tu sever
				temp[lengten] = '\0';

				char tenuser[100];
				int lengtenuser;
				//gui thong tin qua cho sever
				cout << "Email: ";
				cin >> tenuser;
				lengtenuser = strlen(tenuser);
				//gui di dodai thong diep
				client.Send(&lengtenuser, sizeof(lengtenuser), 0);
				//gui thong diep
				client.Send(tenuser, lengtenuser, 0);

				//nhan check
				int check = -1;
				client.Receive((char*)&check, sizeof(int), 0);
				if (id == check)
				{
					cout << "Ten trung moi nhap lai email." << endl;
					//gui user lai
					cout << "Nhap lai email: ";
					cin >> tenuser;
					lengtenuser = strlen(tenuser);
					//gui di dodai thong diep
					client.Send(&lengtenuser, sizeof(lengtenuser), 0);
					//gui thong diep
					client.Send(tenuser, lengtenuser, 0);
				}

				////Nhan Thong Tin San Pham
				int tiensp;
				int idsp;
				int MsgSize;
				char *temp;
				int count;

				client.Receive((char *)&count, sizeof(count), 0);

				cout << "==================== Danh sach cac san pham ban dau gia =====================" << endl;

				for (int i = 0; i < count; i++)
				{
					client.Receive((char*)&idsp, sizeof(idsp), 0);
					cout << idsp << " - ";

					client.Receive((char*)&MsgSize, sizeof(int), 0);
					temp = new char[MsgSize + 1];
					client.Receive((char*)temp, MsgSize, 0);

					//in ra thong diep tu client
					temp[MsgSize] = '\0';
					cout << temp << " - ";

					client.Receive((char*)&tiensp, sizeof(tiensp), 0);
					cout << tiensp << endl;
					delete temp;
				}
				// BAT THOI GIAN HIEN TAI

				time_t baygio = time(0);
				tm *ltm = localtime(&baygio);

				int phuthientai = ltm->tm_min ;
				int giayhientai = ltm->tm_sec;

				int dodai;
				char *temp1;
				int check_break_loop = 0;
				do
				{
					
					// nhan bien check_break_loop
					client.Receive((char*)&check_break_loop, sizeof(int), 0);
					/*cout << "Check_break_loop Variable: " << check_break_loop << endl;*/
					
					if (check_break_loop == 0)
					{

						//nhan thong tin tu user nhan do dai 
						client.Receive((char*)&dodai, sizeof(int), 0);
						temp1 = new char[dodai + 1];
						//nhan chuoi
						client.Receive((char*)temp1, dodai, 0);
						//in ra thong deip tu sever
						temp1[dodai] = '\0';

						cout << "Sever: " << temp1 << endl;


						char tenuser1[100];
						int lengtenuser1;
						//gui thong tin qua cho sever
						cout << "Dau Gia: ";
						cin >> tenuser1;
						lengtenuser1 = strlen(tenuser1);
						//gui di dodai thong diep
						client.Send(&lengtenuser1, sizeof(lengtenuser1), 0);
						//gui thong diep
						client.Send(tenuser1, lengtenuser1, 0);

						int check_in = 1;

						///nhan bien check
						client.Receive(&check_in, sizeof(check_in), 0);

						if (check_in == count)
						{
							cout << "San Pham Khong Ton Tai: " << endl;
							cout << "Dau Gia Lai: ";
							cin >> tenuser1;
							lengtenuser1 = strlen(tenuser1);
							//gui di dodai thong diep
							client.Send(&lengtenuser1, sizeof(lengtenuser1), 0);
							//gui thong diep
							client.Send(tenuser1, lengtenuser1, 0);
						}
					}
				} while (check_break_loop == 0);
				cout << "Het Gio " << endl;

				int num_client;

				client.Receive((char*)&num_client, sizeof(int), 0);
				int counting_temp = num_client;
				/*if (check_break_loop >= 2)
				{
					num_client = check_break_loop;
				}*/
				cout << num_client<<endl;
				
				int count_arr_muahang = 0;
				int check_last_winner_or_loser = 0;
				client.Receive((char *)&count_arr_muahang, sizeof(int), 0);
				/*if (count_arr_muahang == 0)
				{
					count_arr_muahang = counting_temp;
				}*/
				//cout << count_arr_muahang<<endl;
				//tao mang id trung thuong
				int *trungthuong = new int[10];
				for (int i = 0; i < count_arr_muahang; i++)
				{
					int id_client_win_or_lose;
					client.Receive((char*)&id_client_win_or_lose, sizeof(int), 0);
					//cout << id_client_win_or_lose << endl;
					trungthuong[i] = id_client_win_or_lose;

					if (id_client_win_or_lose == id)
					{
						check_last_winner_or_loser = 0;
						break;
					}
					else
					{
						check_last_winner_or_loser++;
					}

				}
				
				char code_bank[100];
				char seri_card[100];
				int length_of_seri_card;
				int length_of_code_bank;

				char khonghi[100] = " ";
				int lengkk;

				if (id == trungthuong[0] || id == trungthuong[1] || id == trungthuong[2] || id == trungthuong[3 || id == trungthuong[4]])
					{
						cout << "You won!" << endl;
						//gui ma ngan hang

						do
						{
							cout << "Nhap ma ngan hang(3 ky tu va A-Z): ";
							cin >> code_bank;
							if (strlen(code_bank) != 3)
							{
								cout << "Nhap Lai Nhe!! " << endl;
							}
						} while (strlen(code_bank)!=3);

						length_of_code_bank = strlen(code_bank);
						//gui di dodai thong diep
						client.Send(&length_of_code_bank, sizeof(length_of_code_bank), 0);
						//gui thong diep
						client.Send(code_bank, length_of_code_bank, 0);

						do
						{
							cout << "Nhap ma the(10 ky tu 0-9): ";
							cin >> seri_card;
							if (strlen(seri_card) != 10)
							{
								cout << "Nhap Lai Nhe!! " << endl;
							}
						} while (strlen(seri_card)!=10);

						length_of_seri_card = strlen(seri_card);
						client.Send(&length_of_seri_card, sizeof(length_of_seri_card), 0);
						//gui thong diep
						client.Send(seri_card, length_of_seri_card, 0);
						cout << "Ban Da Mua Hang Thanh Cong" << endl;
					}
					else
					{
						cout << "You lose!" << endl;
						//gui khong khi qua 

						lengkk = strlen(khonghi);
						//gui di dodai thong diep
						client.Send(&lengkk, sizeof(lengkk), 0);
						//gui thong diep
						client.Send(khonghi, lengkk, 0);

						lengkk = strlen(khonghi);
						//gui di dodai thong diep
						client.Send(&lengkk, sizeof(lengkk), 0);
						//gui thong diep
						client.Send(khonghi, lengkk, 0);
					}
	
				cout << "Ket Thuc Chuong Trinh!!"<<endl;
				
				client.Close();
			}
			else
				printf("Khong connect duoc toi server....");
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
