// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PRNT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PRNT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PRNT_EXPORTS
#define PRNT_API __declspec(dllexport)
#else
#define PRNT_API __declspec(dllimport)
#endif

// �����Ǵ� PRNT.dll ������
class PRNT_API CPRNT {
public:
	CPRNT(void);
	// TODO:  �ڴ��������ķ�����
};

extern PRNT_API int nPRNT;

PRNT_API int fnPRNT(void);