#ifndef _LOG_H_
#define	_LOG_H_

#define	LOG_DIR	"D:\\QmDriver\\Log"

void WriteLog(char *pFile, char *pLog)
{
	try
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		char cDateTime[100] = {0};
		sprintf(cDateTime, "%4.4ld%2.2ld%2.2ld_%2.2ld%2.2ld%2.2ld", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		FILE *fp = fopen(pFile, "ab+");
		fprintf(fp, "[%s]%s\r\n", cDateTime, pLog);
		fflush(fp);
		fclose(fp);
	}
	catch(...)
	{
	}
}


#endif	//_LOG_H_