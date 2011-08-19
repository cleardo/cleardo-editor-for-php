#include "common.h"
#include "lexer.h"

char lex_input_buff[LEX_BUF_HALF_SIZE+1] = "36>=";
int lex_buff_flag = 0;

int lex_getbuff(char lex_temp_buf[])
{
	int i = 0;
	if (!lex_buff_flag) return 0;
  while (lex_input_buff[i] != '\0' && lex_input_buff[i] != EOF)
  {
	lex_temp_buf[i] = lex_input_buff[i];
	i++;
  }
  cout<<"loadsize"<<i<<endl;
  lex_buff_flag = 0;
  return i;


}

void lex_setbuff(char lex_to_buff[])
{
	lex_buff_flag = 1;
	int i = 0;
	while (lex_to_buff[i] != '\0' && lex_to_buff[i] != EOF)
	{
		lex_input_buff[i] = lex_to_buff[i];
		i++;
	}
	lex_input_buff[i] = '\0';

}

void lex_buffer_dump()
{
	ArrayDump(lex_input_buff, 5);
}
