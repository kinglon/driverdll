
#ifndef _CONFIG_H_
#define	_CONFIG_H_

//却除行尾的回车换行两个符号
void StrTrim(char *pStr)
{
	int iStrLen = strlen(pStr);
	if(iStrLen < 2)
	{
		return;
	}

	if((13 == pStr[iStrLen-2]) && (10 == pStr[iStrLen-1]))
	{
		pStr[iStrLen-2] = 0;
		pStr[iStrLen-1] = 0;
	}
}

//读取行配置
bool ReadConfigFile(char *pFile, char *pItem, char *pValue)
{
	FILE *fp;
	char cLine[200];
	char *p;
	bool bFindItem = false;

	fp = fopen(pFile, "rb");
	if(!fp)
	{
		return false;
	}

	while(!feof(fp))
	{
		memset(cLine, 0, 200);
		fgets(cLine, 200, fp);

		StrTrim(cLine);		
		
		if(strlen(cLine) > 0)
		{
			p = strtok(cLine, "=");
			if(0 == strcmp(p, pItem))
			{
				p = strtok(NULL, "=");
				
				if(NULL == p)
				{
					sprintf(pValue, "%s", "");
				}
				else
				{
					sprintf(pValue, "%s", p);
				}

				bFindItem = true;
				
				break;
			}
		}
	}

	fclose(fp);

	if(false == bFindItem)
	{
		return false;
	}

	return true;
}


#endif	//_CONFIG_H_