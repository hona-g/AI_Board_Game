
// DdangDdaMuckgiDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "DdangDdaMuckgi.h"
#include "DdangDdaMuckgiDlg.h"
#include "afxdialogex.h"
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int clk[10] = { 0 };
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDdangDdaMuckgiDlg ��ȭ ����



CDdangDdaMuckgiDlg::CDdangDdaMuckgiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDdangDdaMuckgiDlg::IDD, pParent)
	, x_point1(0)
	, x_point2(0)
	, y_point1(0)
	, y_point2(0)
	, red_score(0)
	, blue_score(0)
	, count(0)
	, polygon(true)
	, rnum(0)
	, team_name(_T("RED"))
	, histo(_T(""))
	, chk(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int temp = 0;;

	m_view = cvCreateImage(cvSize(700, 700), 8, 3);

	srand(3);

	for (int i = 0; i<7; i++)
		for (int j = 0; j<7; j++)
			map[i][j] = rand() % 2;


	for (int i = 0; i < 700; i++){
		for (int j = 0; j < 700; j++){

			m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)255;
		}
	}

	for (int i = 0; i<700; i++){
		for (int j = 0; j<700; j++){
			if (i % 100 == 0 || j % 100 == 0){
				m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)200;
			}
			else if (i % 50 == 0 && j % 50 == 0)
				if (map[((j - 50) / 100)][((i - 50) / 100)] == 1){
					for (int y = -5; y <= 5; y++)
						for (int x = -5; x <= 5; x++)
						{
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 2] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 1] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3] = (unsigned char)0;
						}
					temp++;
				}
		}
	}

}

void CDdangDdaMuckgiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW, m_iView);
	DDX_Text(pDX, IDC_X1, x_point1);
	DDV_MinMaxInt(pDX, x_point1, 1, 7);
	DDX_Text(pDX, IDC_X2, x_point2);
	DDV_MinMaxInt(pDX, x_point2, 1, 7);
	DDX_Text(pDX, IDC_Y1, y_point1);
	DDV_MinMaxInt(pDX, y_point1, 1, 7);
	DDX_Text(pDX, IDC_Y2, y_point2);
	DDV_MinMaxInt(pDX, y_point2, 1, 7);
	DDX_Text(pDX, IDC_RED_SCORE, red_score);
	DDX_Text(pDX, IDC_BLUE_SCORE, blue_score);
	DDX_Text(pDX, IDC_TEAM_NAME, team_name);
	DDX_Text(pDX, IDC_HISTO, histo);
}

BEGIN_MESSAGE_MAP(CDdangDdaMuckgiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_INPUT, &CDdangDdaMuckgiDlg::OnBnClickedInput)
	ON_BN_CLICKED(IDC_RESTART, &CDdangDdaMuckgiDlg::OnBnClickedRestart)
	ON_WM_KEYDOWN()
	//	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// CDdangDdaMuckgiDlg �޽��� ó����

BOOL CDdangDdaMuckgiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetTimer(1, (UINT)((1 / (double)12) * 10), NULL);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDdangDdaMuckgiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDdangDdaMuckgiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_View.CopyOf(m_view);

		m_iView.GetClientRect(&view);
		m_View.DrawToHDC(m_iView.GetDC()->GetSafeHdc(), view);

		CDialogEx::OnPaint();
	}


}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDdangDdaMuckgiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDdangDdaMuckgiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (1 == nIDEvent){
		if (polygon)
		{
			for (int i = 0; i < triangle.size(); i++)
			{

				CvPoint cvp[4];
				cvp[0].x = triangle[i]->pt[0].x * 100 + 50;
				cvp[0].y = triangle[i]->pt[0].y * 100 + 50;
				cvp[1].x = triangle[i]->pt[1].x * 100 + 50;
				cvp[1].y = triangle[i]->pt[1].y * 100 + 50;
				cvp[2].x = triangle[i]->pt[2].x * 100 + 50;
				cvp[2].y = triangle[i]->pt[2].y * 100 + 50;
				CvPoint *parr[2] = { &cvp[0], 0 };
				int vernum[2] = { 3, 0 };
				int polynum = 1;

				int r = 100, g = 100, b = 100;
				if (triangle[i]->owner == 1) r = 255;
				if (triangle[i]->owner == 0) b = 255;


				/*if (i < rnum)
				{
				r = g = b = 150;
				}
				else if (i == rnum)
				{
				r = g = b = 0;
				}
				else continue;*/

				cvFillPoly(m_view, parr, vernum, polynum, CV_RGB(r, g, b));
			}
		}
		for (int i = 0; i<count; i++){

			if (i % 2 == 0)
				cvLine(m_view, cvPoint(line[i][0] * 100 - 50, line[i][1] * 100 - 50), cvPoint(line[i][2] * 100 - 50, line[i][3] * 100 - 50), CV_RGB(255, 0, 0), 2);
			else
				cvLine(m_view, cvPoint(line[i][0] * 100 - 50, line[i][1] * 100 - 50), cvPoint(line[i][2] * 100 - 50, line[i][3] * 100 - 50), CV_RGB(0, 0, 255), 2);

		}

		if (chk == true){
			m_View.CopyOf(m_view);

			m_iView.GetClientRect(&view);
			m_View.DrawToHDC(m_iView.GetDC()->GetSafeHdc(), view);

			chk = false;

		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

int gcd(int a, int b)
{
	if (a == 0) return b;
	if (b == 0) return a;
	return gcd(b, a%b);
}

int CDdangDdaMuckgiDlg::CheckDotCross(int x1, int y1, int x2, int y2)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	if (dx == 0 && dy == 0) return 1;

	int sx = x1, sy = y1;
	int ex = x2, ey = y2;

	int g = gcd(dx, dy);
	int cdx = dx / g;
	int cdy = dy / g;

	if (x1 > x2) cdx = -cdx;
	if (y1 > y2) cdy = -cdy;

	for (int x = sx + cdx, y = sy + cdy; x != ex || y != ey; x += cdx, y += cdy)
	{
		if (x == ex && y == ey) break;
		if (map[x - 1][y - 1] == 1) return 1;
	}
	return 0;
}
// �ݼ�ó��2 - ���� �����Ǵ°�
int CDdangDdaMuckgiDlg::CheckLineCrossOneToOne(CPoint s1, CPoint e1, CPoint s2, CPoint e2)
{
	int a1 = s1.x; int b1 = s2.x;
	int a2 = s1.y; int b2 = s2.y;
	int a3 = e1.x; int b3 = e2.x;
	int a4 = e1.y; int b4 = e2.y;
	float x1 = a1, x2 = a3, x3 = b1, x4 = b3;
	float y1 = a2, y2 = a4, y3 = b2, y4 = b4;
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4); // ����

	if (s1 == e2)
		if (s2 == e1)
			return true;

	if (d == 0) // d=0 �̸� ���� �Ǵ� ��ġ
		return false;   // (������ ���)

	// �������� ��ǥ ���ϱ� (���弱�� ������)
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	if (x > min(x1, x2) && x < max(x1, x2) &&
		x > min(x3, x4) && x < max(x3, x4))
		return true; // ù ��° ���� ���ο� ���� ���� ���
	if (y > min(y1, y2) && y < max(y1, y2) &&
		y > min(y3, y4) && y < max(y3, y4))
		return true; // �� ��° ���� ���ο� ���� ���� ���
	if (x1 == x2)
		if (y3 == y4)
			if (min(x3, x4) < x1 && x1<max(x3, x4))
				if (min(y1, y2) < y3 && y3<max(y1, y2))
					return true;
	if (x3 == x4)
		if (y1 == y2)
			if (min(x1, x2) < x3 && x3<max(x1, x2))
				if (min(y3, y4) < y1 && y1<max(y3, y4))
					return true;

	return false; // ���ο� �ִ� ���
}

int CDdangDdaMuckgiDlg::CheckLineCross(CPoint s1, CPoint e1)
{
	for (int i = 0; i < count; i++)
	{
		clk[5] ++;

		CPoint s2 = CPoint(line[i][0], line[i][1]);
		CPoint e2 = CPoint(line[i][2], line[i][3]);
		if (CheckLineCrossOneToOne(s1, e1, s2, e2) == true)
		{
			return 1;
		}
	}
	return 0;
}

int CDdangDdaMuckgiDlg::CheckSameLineOneToOne(CPoint s1, CPoint e1, CPoint s2, CPoint e2)
{
	if (s1 == s2 && e1 == e2) return 1;
	if (s1 == e2 && e1 == s2) return 1;
	return 0;
}

int CDdangDdaMuckgiDlg::CheckSameLine(CPoint s1, CPoint e1)
{
	for (int i = 0; i < count; i++)
	{
		clk[4] ++;

		CPoint s2 = CPoint(line[i][0], line[i][1]);
		CPoint e2 = CPoint(line[i][2], line[i][3]);
		if (CheckSameLineOneToOne(s1, e1, s2, e2) == true)
		{
			return 1;
		}
	}
	return 0;
}

void CDdangDdaMuckgiDlg::OnBnClickedInput()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	AddLine(x_point1, y_point1, x_point2, y_point2, 1);
	//AI();
	//for (int i = 0; i < 100000; i++)
	//{
	//	int t = AddLine(1 + rand() % 7, 1 + rand() % 7, 1 + rand() % 7, 1 + rand() % 7, 0);
	//	//if (t) break;
	//}
}

int CDdangDdaMuckgiDlg::AddTriangle(CPoint p1, CPoint p2, int team)
{
	int cnt = 0;

	for (int i = 0; i < via[p1.x][p1.y].size(); i++)
	{
		for (int j = 0; j < via[p2.x][p2.y].size(); j++)
		{
			clk[3] ++;

			if (via[p1.x][p1.y][i] == via[p2.x][p2.y][j]) // �������� ����� ���� ������ �ﰢ��
			{
				CPoint intersection = via[p1.x][p1.y][i];
				Triangle *t = new Triangle(p1, p2, intersection);
				if (CheckTriangle(t) == true)
				{
					triangle.push_back(t);
					t->owner = team;

					if (team == 0)
					{
						blue_score++;
					}
					else
					{
						red_score++;
					}
					cnt++;
				}
			}
		}
	}

	CheckTriangle(p1);
	CheckTriangle(p2);
	return cnt;
}

bool CDdangDdaMuckgiDlg::CheckTriangle(Triangle *p) // �ﰢ���� ��ȿ�Ѱ�? (�ﰢ�� �������� ������ ���� ����� ��츦 üũ)
{
	for (int i = 0; i < 3; i++)
	{
		CPoint pt = p->pt[i];
		for (int j = 0; j < via[pt.x][pt.y].size(); j++)
		{
			clk[2] ++;

			CPoint row = via[pt.x][pt.y][j];
			if (row == p->pt[0] || row == p->pt[1] || row == p->pt[2])
				continue;

			CRgn rgn;
			CPoint rpt[4];
			rpt[0] = p->pt[0];
			rpt[1] = p->pt[1];
			rpt[2] = p->pt[2];
			rpt[3] = p->pt[0];
			rgn.CreatePolygonRgn(p->pt, 3, WINDING);

			if (rgn.PtInRegion(row)) // row �� p->pt[0~2] �� �̷�� �ﰢ���� ���ο� �����Ѵٸ� ***
			{
				return false;
			}
		}
	}
	return true;
}

void CDdangDdaMuckgiDlg::DeleteTriangle(Triangle *t)
{
	for (auto iter = triangle.begin(); iter != triangle.end();)
	{
		clk[1] ++;

		if ((*iter) == t)
		{
			//delete (*iter);
			iter = triangle.erase(iter);
		}
		else iter++;
	}
}

bool CDdangDdaMuckgiDlg::CheckTriangle(CPoint p) // ���� �־����� �� ���� �����ϴ� �ﰢ���� ��ȿ�Ѱ�
{
	for (int i = 0; i < triangle.size(); i++)
	{
		clk[0] ++;

		Triangle *t = triangle[i];
		if (t->has(p))
		{
			if (CheckTriangle(t) == false)
			{
				if (t->owner == 1)
					red_score--;
				else blue_score--;

				DeleteTriangle(t);
			}
		}
	}
	return true;
}


int CDdangDdaMuckgiDlg::AddLine(int x1, int y1, int x2, int y2, bool emsg)
{
	clk[6]++;
	int score = 0;
	if (map[x1 - 1][y1 - 1] == 1 && map[x2 - 1][y2 - 1] == 1){

		int temp = 0;

		for (int i = 0; i<count/* + 1*/; i++)
		{
			temp = 0;

			if (line[i][0] == x1)
				temp++;
			if (line[i][1] == y1)
				temp++;
			if (line[i][2] == x2)
				temp++;
			if (line[i][3] == y2)
				temp++;

			if (temp == 4){
				if (emsg) MessageBox(_T("Error."));
				return 0;
			}
		}

		if (temp != 4){

			int dotCross = CheckDotCross(x1, y1, x2, y2);
			int lineCross = CheckLineCross({ x1, y1 }, { x2, y2 });

			if (dotCross == 1)
			{
				if (emsg) MessageBox(_T("Error. Another Dot Crossed"));
				return 0;
			}
			if (lineCross == 1)
			{
				if (emsg) MessageBox(_T("Error. Another Line Crossed"));
				return 0;
			}

			line[count][0] = x1;
			line[count][1] = y1;
			line[count][2] = x2;
			line[count++][3] = y2;

			via[x1 - 1][y1 - 1].push_back({ x2 - 1, y2 - 1 });
			via[x2 - 1][y2 - 1].push_back({ x1 - 1, y1 - 1 });
			score = AddTriangle({ x1 - 1, y1 - 1 }, { x2 - 1, y2 - 1 }, count % 2);

		}


		// 
		// i��° �Էµ� X1 : line[i][0], Y1 : line[i][1], X2 : line[i][2], Y2 : line[i][3]
		// ���� �Էµ� X1 : x_point1, Y1 : y_point1, X2 : x_point2, Y2 : y_point2;
		// map[����][����] : 7x7�� ���� ��Ʈ������ ����� ��� 0, ���� ��� 1
		// count ������� �Էµ� ������ ��
		// team_name ���� �Է°����� �� �̸�
		// histo ���ݱ��� �Էµ� ���� ���� ��ǥ��
		// red_score red�� ����
		// blue_score blue�� ����
		// 
		CString str;
		str.Format(_T("%s : (%d, %d), (%d, %d)\r\n"), team_name, x1, y1, x2, y2);
		histo += str;
		UpdateData(false);

		if (count % 2 == 0)
			team_name = _T("RED");
		else
			team_name = _T("BLUE");

		chk = true;
	}
	else
	{
		if (emsg) MessageBox(_T("Error."));
		return 0;
	}

	return 1;
}

void CDdangDdaMuckgiDlg::OnBnClickedRestart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//srand((unsigned int)time(NULL));
	triangle.clear();
	for (int i = 0; i<7; i++)
		for (int j = 0; j < 7; j++)
		{
			map[i][j] = rand() % 2;
			via[i][j].clear();
		}


	for (int i = 0; i < 700; i++){
		for (int j = 0; j < 700; j++){

			m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)255;
		}
	}

	for (int i = 0; i<700; i++){
		for (int j = 0; j<700; j++){

			if (i % 100 == 0 || j % 100 == 0){
				m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)200;
			}
			else if (i % 50 == 0 && j % 50 == 0)
				if (map[((j - 50) / 100)][((i - 50) / 100)] == 1){
					for (int y = -5; y <= 5; y++)
						for (int x = -5; x <= 5; x++)
						{
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 2] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 1] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3] = (unsigned char)0;
						}
				}
		}
	}

	red_score = 0;
	blue_score = 0;
	count = 0;
	team_name = _T("RED");
	histo = _T("");
	chk = true;

	UpdateData(FALSE);
}


BOOL CDdangDdaMuckgiDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	polygon = !polygon;
	rnum++; if (rnum >= triangle.size()) rnum = 0;

	for (int i = 0; i < 700; i++){
		for (int j = 0; j < 700; j++){

			m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)255;
			m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)255;
		}
	}

	for (int i = 0; i<700; i++){
		for (int j = 0; j<700; j++){

			if (i % 100 == 0 || j % 100 == 0){
				m_view->imageData[(i*m_view->width + j) * 3 + 2] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3 + 1] = (unsigned char)200;
				m_view->imageData[(i*m_view->width + j) * 3] = (unsigned char)200;
			}
			else if (i % 50 == 0 && j % 50 == 0)
				if (map[((j - 50) / 100)][((i - 50) / 100)] == 1){
					for (int y = -5; y <= 5; y++)
						for (int x = -5; x <= 5; x++)
						{
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 2] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3 + 1] = (unsigned char)0;
							m_view->imageData[((i + y)*m_view->width + (j + x)) * 3] = (unsigned char)0;
						}
				}
		}
	}
	chk = true;
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDdangDdaMuckgiDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if ((point.x - 60) / 70 + 1 <= 7)
	{
		x_point1 = (point.x - 60) / 70 + 1;
		y_point1 = (point.y - 70) / 70 + 1;
	}
	UpdateData(false);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDdangDdaMuckgiDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if ((point.x - 60) / 70 + 1 <= 7)
	{
		x_point2 = (point.x - 60) / 70 + 1;
		y_point2 = (point.y - 70) / 70 + 1;
	}
	UpdateData(false);
	CDialogEx::OnRButtonDown(nFlags, point);
}

BackupData *CDdangDdaMuckgiDlg::CreateBackup()
{
	/// --------	 ����� ������ ���� --------------
	BackupData *bd = new BackupData();

	/// --------	 ������ ��� --------------

	for (int i = 0; i < 160; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			bd->temp_line[i][j] = line[i][j];
		}
	}
	bd->temp_count = count;
	bd->temp_chk = chk;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			bd->temp_map[i][j] = map[i][j];
			bd->temp_via[i][j].clear();
			for (int k = 0; k < via[i][j].size(); k++)
			{
				bd->temp_via[i][j].push_back(via[i][j][k]);
			}
		}
	}

	for (int i = 0; i < triangle.size(); i++)
		bd->temp_triangle.push_back(triangle[i]);

	bd->temp_red_score = red_score;
	bd->temp_blue_score = blue_score;

	return bd;
}

void CDdangDdaMuckgiDlg::SetFromBackup(BackupData *bd)
{
	/// --------	 ������ ���� --------------

	for (int i = 0; i < 160; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			line[i][j] = bd->temp_line[i][j];
		}
	}
	count = bd->temp_count;
	chk = bd->temp_chk;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			map[i][j] = bd->temp_map[i][j];
			via[i][j].clear();
			for (int k = 0; k < bd->temp_via[i][j].size(); k++)
			{
				via[i][j].push_back(bd->temp_via[i][j][k]);
			}
		}
	}

	triangle.clear();
	for (int i = 0; i < bd->temp_triangle.size(); i++)
		triangle.push_back(bd->temp_triangle[i]);

	red_score = bd->temp_red_score;
	blue_score = bd->temp_blue_score;
}

int CDdangDdaMuckgiDlg::AI()
{
	/// --------	 ������ ��� --------------
	//BackupData *backup = CreateBackup();
	/// --------	 �ų��� �����͸� ���� --------------

	MonteCarloAI();

	/// --------	 ������ ���� --------------
	//SetFromBackup(backup);
	/// -----------������ ������ �� �߱�--------
	return 0;
}

bool CDdangDdaMuckgiDlg::CanConnect(int x1, int y1, int x2, int y2)
{
	if (map[x1 - 1][y1 - 1] == 0 || map[x2 - 1][y2 - 1] == 0) return 0;
	if (CheckSameLine({ x1, y1 }, { x2, y2 }) == true) return 0;
	if (CheckDotCross(x1, y1, x2, y2) == true) return 0;
	if (CheckLineCross({ x1, y1 }, { x2, y2 }) == true) return 0;
	return 1;
}

int CDdangDdaMuckgiDlg::WillBeTriangle(int x1, int y1, int x2, int y2)
{
	if (map[x1-1][y1-1] == 0 || map[x2-1][y2-1] == 0) return 0;
	if (CheckSameLine({ x1, y1 }, { x2, y2 }) == true) return 0;
	if (CheckDotCross(x1, y1, x2, y2) == true) return 0;
	if (CheckLineCross({ x1, y1 }, { x2, y2 }) == true) return 0;
	
	CPoint s = { x1-1, y1-1 };
	CPoint e = { x2-1, y2-1 };

	int tri = 0;
	for (int i = 0; i < via[s.x][s.y].size(); i++)
	{
		// via(via(s)) �� ��� ������ e�� ������ �ﰢ�� ++
		CPoint v = via[s.x][s.y][i];
		for (int j = 0; j < via[v.x][v.y].size(); j++)
		{
			CPoint vv = via[v.x][v.y][j];
			if (vv == e) tri++;
		}
	}

	return tri;
}

int CDdangDdaMuckgiDlg::WillBeTriangleEx(int x1, int y1, int x2, int y2, int a1, int b1, int a2, int b2)
{
	int tri = 0;
	CPoint s1 = { x1 , y1 };
	CPoint e1 = { x2 , y2 };
	CPoint s2 = { a1 , b1 };
	CPoint e2 = { a2 , b2 };

	if (s1 == e1)
	{
		// s2 - e2 �������� ����?
		return CanConnect(s2.x,s2.y,e2.x,e2.y);
	}
	else if (s1 == e2)
	{
		// s2 - e1 �������� ����?
		return CanConnect(s2.x, s2.y, e1.x, e1.y);
	}
	else if (s2 == e1)
	{
		// s1 - e2 �������� ����?
		return CanConnect(s1.x, s1.y, e2.x, e2.y);
	}
	else if (s2 == e2)
	{
		// s1 - e1 �������� ����?
		return CanConnect(s1.x, s1.y, e1.x, e1.y);
	}

	return 0;
}

void CDdangDdaMuckgiDlg::MonteCarloAI()
{
	// �ﰢ���� �׸� �� �ִ� ��(����A) ã��
	for (int i = 0; i < 10000; i++)
	{
		int s1 = 1 + rand() % 7;
		int e1 = 1 + rand() % 7;
		int s2 = 1 + rand() % 7;
		int e2 = 1 + rand() % 7;
		int t = WillBeTriangle(s1,e1,s2,e2);

		if (t > 0) // a(A�� ����)�� �׾����� ���� ���� �� �ִ°�
		{
			BackupData *backup = CreateBackup();
			int added = AddLine(s1, e1, s2, e2, false);
			bool profit = true;
			
			for (int j = 0; j < 1000; j++)
			{
				int _s1 = 1 + rand() % 7;
				int _e1 = 1 + rand() % 7;
				int _s2 = 1 + rand() % 7;
				int _e2 = 1 + rand() % 7;
				int _t = WillBeTriangle(_s1, _e1, _s2, _e2);
				if (_t > t)
				{
					SetFromBackup(backup);
					delete backup;
					profit = false;
					break;
				}
			}

			if (profit)
			{
				delete backup;
				return;
			}
		}
	}

	// ������ a�� ���ٸ� ���� �ﰢ���� �� �߰� �� �� �ִ°�
	for (int i = 0; i < 10000; i++)
	{
		int s1 = 1 + rand() % 7;
		int e1 = 1 + rand() % 7;
		int s2 = 1 + rand() % 7;
		int e2 = 1 + rand() % 7;
		BackupData *backup = CreateBackup();
		int added = AddLine(s1, e1, s2, e2,false);
		if (!added)
		{
			delete backup;
			continue;
		}

		bool enemyavail = false;
		for (int j = 0; j < 100; j++)
		{
			int _s1 = 1 + rand() % 7;
			int _e1 = 1 + rand() % 7;
			int _s2;
			int _e2;

			if (rand() % 2)
			{
				_s2 = s1;
				_e2 = e1;
			}
			else
			{
				_s2 = s2;
				_e2 = e2;
			}
				
			int _t = WillBeTriangle(_s1, _e1, _s2, _e2);
			// �ذ� Ex�Լ� -> s1, e1, s2, e2 �� �����Ͽ� �ﰢ���� �� �� �ִ°�
			if (_t > 0)
			{
				enemyavail = true;
				break;
			}
		}

		if (enemyavail == true)
		{
			SetFromBackup(backup);
			delete backup;
		}
		else
		{
			delete backup;
			return;
		}
	}

	int gg = 0;
	// �ƹ� ���ǵ� ���ٸ� �׳� �������� ����
}


void CDdangDdaMuckgiDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	AI();
	chk = true;
	CDialogEx::OnMButtonDown(nFlags, point);
}
