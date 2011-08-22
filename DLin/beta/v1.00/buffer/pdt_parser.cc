/**
 * 语法产生式解析器函数实现
 */
void factor()
{
    if (pdt_isTerminal(token))
    {
	nexttoken();
    }
    /*
    else
    {
	if (token == LEFT_SQBRACKET)
	{
	    nexttoken();
	    pdt_term();
	    if (token == RIGHT_SQBRACKET)
	    {
		nexttoken();
	    }
	}
    }
    */
}