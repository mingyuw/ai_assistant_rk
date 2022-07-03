#include <ctime>
#include "nunova_mix.h"

static int g_isPrint = 1;
static int g_isSave = 1;

std::string GetTimeToStringForLog()
{
    char timeStr[32] = {0};
    struct std::tm cutTm = {0};
    std::time_t nowTime = std::time(NULL); // std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());//转换为 std::time_t 格式
    std::strftime(timeStr, 32, "%Y-%m-%d_%X", localtime_r(&nowTime, &cutTm));
    std::string time_Str = timeStr;

    return time_Str;
}

std::string GetTimeToStringForPrint()
{
    char timeStr[32] = {0};
    struct std::tm cutTm = {0};
    std::time_t nowTime = std::time(NULL); // std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());//转换为 std::time_t 格式
    std::strftime(timeStr, 32, "%Y-%m-%d_%H:%M:%S ", localtime_r(&nowTime, &cutTm));
    std::string time_Str = timeStr;

    return time_Str;
}


void WriteCSV(std::string filename, cv::Mat m)
{
   std::ofstream myFile;
   myFile.open(filename.c_str());
   myFile << cv::format(m, cv::Formatter::FMT_CSV) << std::endl;
   myFile.close();
   return;
}



int AddInt(const int n, ...)
{
	va_list arg;//定义va_list类型变量arg
 
	va_start(arg,n);//arg初始化,n是第一个可见参数名字;使arg指向第一个可选参数
	
	int sum=0;
	int tmp =0;
	for(int i=0;i<n;i++)
	{
		tmp=va_arg(arg,int);//返回参数列表中的当前参数并使arg指向参数列表中的下一个参数
		sum+=tmp;
	}
	
	va_end(arg);//把arg指针清为NULL
	
	return sum;
}


const double eps = 1e-12;
//字符串首尾调换
void Reverse(char *s)
{
     int i , j;
     int len = strlen(s);
     for( i = 0 , j = len - 1 ; i < j ; i++ , j--)
         s[i]^=s[j]^=s[i]^=s[j];
}

/* Ftoa:   convert double n to string s with digits*/
void Ftoa(double n, char *s,  int digits)
{
    memset(s , 0 , sizeof(s));
    int i = 0 , p;
    bool sign = true;
    if( n < 0 ) 
    { 
        sign = false;
        n = -n;
    }
    long   t = ( long)n;
    n = n - t;
    if( t == 0)
    {
        s[i++] = '0';
    }
    else
    {
        while( t != 0 )
        {
            p = t % 10;
            t /= 10;
            s[i++] = p + '0';
        }
        if(!sign) s[i++] = '-';
        Reverse(s);
    }
    if( fabs( n - 0.0 ) > eps )
    {
        s[i++] = '.';
        int cur_d = 0;
        while( cur_d < digits)
        {
            cur_d++;
            n *= 10.0;
            p = (long )n;
            n = n - p;
            s[i++] = p + '0';
        }
    }
    s[i] = '\0';
}


/*
int MyPrintf(const char *format, ...)
{
	va_list	ap;
	va_start(ap,format);
	
	//STRING
	//char ch;
	 const char *str;
	 //const char *str1;
	
	//OCT
	int value ;
	char buf[MAXBUF];
 
	//HEX
	int offset;
	unsigned char ucI,ucLen;
	unsigned long ulValue;
	//unsigned char ucBuf[MAXBUF];
	//unsigned long *ulpValue;
 
	//FLOAT
	double fdata;
 
	//BINARY
 
 
	
	while(*format)
	{
		if('%'!=*format)
		{
			putchar(*format++);
			continue;
		}
 
		switch(*++format)
		{
			case	's':
						str = NULL;
						str=va_arg(ap, const char *);
						for(;*str!='\0';str++)
						{
							putchar(*str);
						}
						++format;//越过当前参数，移到下一个参数
						break;
 
			case	'd':
						value=va_arg(ap, int);
						memset(buf,0,sizeof(buf));
						itoa(value, buf, 10);
						for (str=buf ;*str!='\0'; str++) 
						{
							putchar(*str);
						}
						++format;
						break;
			case	'x':
						offset = 0;
						ucLen = 0 ;
						ulValue=va_arg(ap, int);
						memset(buf,0,sizeof(buf));
 
						if( !ulValue )
						{
							buf[ucLen] = ulValue ;
							str = buf ;
							putchar((*str+48));
							++format;
							break;
						}
 
						for( ucI=0;ucI<MAXBUF;ucI++)
						{
							if( pow(16,ucI)>ulValue )
							{
								ucLen = ucI;
								break;
							}
						}
						
						
						for( ucI=0;ucI<ucLen;ucI++)
						{
 
							buf[ucI] = ulValue/pow(16,ucLen-ucI-1) ;
							ulValue = ulValue%(unsigned long)pow(16,ucLen-ucI-1);
 
						}
						
						//puts("0x");
						for (str=buf ;*str!='\0'; str++) 
						{
							if( (*str>=0) && (*str<=9) )
							{
								putchar((*str+48));
							}
							else
							{
								putchar((*str+87));
							}
						}
						++format;
						break;
			case	'X':
						offset = 0;
						ucLen = 0 ;
						ulValue=va_arg(ap,const int);
						memset(buf,0,sizeof(buf));
 
						
						if( !ulValue )
						{
							buf[ucLen] = ulValue ;
							str = buf ;
							putchar((*str+48));
							++format;
							break;
						}
 
						for( ucI=0;ucI<MAXBUF;ucI++)
						{
							if( pow(16,ucI)>ulValue )
							{
								ucLen = ucI;
								break;
							}
						}
						
						
						for( ucI=0;ucI<ucLen;ucI++)
						{
 
							buf[ucI] = ulValue/pow(16,ucLen-ucI-1) ;
							ulValue = ulValue%(unsigned long)pow(16,ucLen-ucI-1);
 
						}
						
						//puts("0X");
						for (str=buf ;*str!='\0'; str++) 
						{
							if( (*str>=0) && (*str<=9) )
							{
								putchar((*str+48));
							}
							else
							{
								putchar((*str+55));
							}
						}
						++format;
						break;
			case	'f':
						fdata = va_arg(ap, double);
						memset(buf,0,sizeof(buf));
						Ftoa(fdata, buf, DEFAULT_PRECI);
						for (str=buf ;*str!='\0'; str++) 
						{
							putchar(*str);
						}
						++format;
						break;
			case	'.':
						if( isdigit(*++format) )
						{
							if( 'f'==(*++format) )
							{
								--format;
								fdata = va_arg(ap, double);
								memset(buf,0,sizeof(buf));
								int preci =*format-'0'; 
								Ftoa(fdata, buf, preci);
								for (str=buf ;*str!='\0'; str++) 
								{
									putchar(*str);
								}
								++format;
								++format;
							}
						}
						else
						{
							--format;
							if( 'f'==(*++format) )
							{
								fdata = va_arg(ap, double);
								memset(buf,0,sizeof(buf));
								int preci =1; 
								Ftoa(fdata, buf, preci);
								for (str=buf ;*str!='\0'; str++) 
								{
									putchar(*str);
								}
								++format;
							}
						}
						break;
			case	'b':
						value=va_arg(ap,const int);
						memset(buf,0,sizeof(buf));
						itoa(value, buf, 2);
						for (str=buf ;*str!='\0'; str++) 
						{
							putchar(*str);
						}
						++format;
						break;
		
 
			default	:	
						break;
		
		}
 
		
 
	}
	
	va_end(ap);
 
	return 0;
}
*/

void OffDebug()
{
    g_isPrint = int(eLogLevel::LL_NULL);
	g_isSave = 0;
    return;
}

void OnDebug()
{
    g_isPrint = int(eLogLevel::LL_DBG);
	g_isSave = 1;
    return;
}

void PrintNORMAL(const char *format, ...)
{
    if(g_isPrint != int(eLogLevel::LL_NULL))
    {
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }

    return;
}

void PrintDBG(const char *format, ...)
{
    if(g_isPrint >= int(eLogLevel::LL_DBG))
    {
        va_list ap;
        va_start(ap, format);
		char tmpS[256];
		snprintf(tmpS, 256, COLOR_DBLUE"[NUNOVA DEBUG][%s]:%s" COLOR_NOCOLOR, GetTimeToStringForPrint().c_str(), format);
        vprintf(tmpS, ap);
        va_end(ap);
    }

    return;
}

void PrintINFO(const char *format, ...)
{
    if(g_isPrint >= int(eLogLevel::LL_INFO))
    {
        va_list ap;
        va_start(ap, format);
		char tmpS[256];
		snprintf(tmpS, 256, COLOR_GREEN"[NUNOVA INFO][%s]:%s" COLOR_NOCOLOR, GetTimeToStringForPrint().c_str(), format);
        vprintf(tmpS, ap);
        va_end(ap);
    }

    return;
}

void PrintWARN(const char *format, ...)
{
    if(g_isPrint >= int(eLogLevel::LL_WARN))
    {
        va_list ap;
        va_start(ap, format);
		char tmpS[256];
		snprintf(tmpS, 256, COLOR_YELLOW"[NUNOVA WARN][%s]:%s" COLOR_NOCOLOR, GetTimeToStringForPrint().c_str(), format);
        vprintf(tmpS, ap);
        va_end(ap);
    }

    return;
}

void PrintERR(const char *format, ...)
{
    if(g_isPrint >= int(eLogLevel::LL_ERR))
    {
        va_list ap;
        va_start(ap, format);
		char tmpS[256];
		snprintf(tmpS, 256, COLOR_RED"[NUNOVA ERROR][%s]:%s" COLOR_NOCOLOR, GetTimeToStringForPrint().c_str(), format);
        vprintf(tmpS, ap);
        va_end(ap);
    }

    return;
}

int GetIsSave()
{
	return g_isSave;
}

void SetLogLevel(int level)
{
	if(level >= (eLogLevel::LL_NULL) && level <= int(eLogLevel::LL_DBG))
	{
		g_isPrint = level;
	}

	return;
}