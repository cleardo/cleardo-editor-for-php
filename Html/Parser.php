<?php
require_once('Html/constants.php');
class Html_Parser
{
	const TEXT_LENGTH = 100;
	protected $_content;
	protected $_lexer;
	protected $_final;
	protected $_pureText;
	protected $_pureLength = 0;
	protected $_isEnough = false;
	
	public function __construct()
	{
		mb_internal_encoding("UTF-8");
		$this->_lexer = new Html_Lexer();	// 初始化词法分析器
	}
	
	public function setContent($content)
	{
		$this->_content = $content;
		$this->_content .= "\0";
		// echo $content;
	}
	
	public function parse()
	{
		$this->_pureText = "";
		$this->_lexer->text = $this->_content;
		$this->_lexer->getsym();
		/*
		while ($this->_lexer->token != DONE) {
			$this->element();
		}
		*/
		$this->doc();
		// echo $this->_lexer->token;
		// $this->_lexer->getsym();
		// echo "id:" . $this->_lexer->id;
		// echo $this->_pureText;
		// echo $this->_pureLength;
	}
	
	public function doc()
	{
		if ($this->_isEnough) {
			return;
		}
		if ($this->_lexer->token == HTML_TEXT) {
			$this->text();
			$this->doc();
		} elseif ($this->_lexer->token == HTML_BEGIN_DIV) {
			$this->match(HTML_BEGIN_DIV);
			$this->doc();
			$this->match(HTML_END_DIV);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_BEGIN_P) {
			$this->match(HTML_BEGIN_P);
			$this->doc();
			$this->match(HTML_END_P);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_BEGIN_PRE) {
			$this->match(HTML_BEGIN_PRE);
			$this->doc();
			$this->match(HTML_END_PRE);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_BEGIN_HREF) {
			$this->match(HTML_BEGIN_HREF);
			$this->doc();
			$this->match(HTML_END_HREF);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_IMG) {
			$this->match(HTML_IMG);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_BR) {
			$this->match(HTML_BR);
			$this->doc();
		} elseif ($this->_lexer->token == HTML_UNDEFINED) {
			$this->_lexer->getsym();
			$this->doc();
		} else {
			
		}
	}
	
	/**
	 * 解析html中的文本串
	 *
	 */
	public function text()
	{
		if ($this->_lexer->token == HTML_TEXT) {
			if ($this->_pureLength < self::TEXT_LENGTH) {
				if (mb_strlen(trim($this->_lexer->id)) != 0) {
					$this->_lexer->id = str_replace("&nbsp;", "", $this->_lexer->id);
					$this->_pureText .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" . trim($this->_lexer->id) . '<br />';
					$this->_pureLength += mb_strlen($this->_lexer->id);
				}
			} else {
				// $this->_pureText = mb_substr($this->_pureText, 0, self::TEXT_LENGTH) . "……";
				// $this->_pureLength = mb_strlen($this->_pureText);
				$this->_isEnough = true;
			}
			$this->match(HTML_TEXT);
		}
	}
	
	public function match($token)
	{
		if ($this->_lexer->token == $token) {
			// echo "token:" . $this->_lexer->token . '|| ' . $this->_lexer->id . "</p>";
			$this->_lexer->getsym();
		} elseif ($this->_isEnough) {
			return;
		} else {
			echo "html验证失败";
			echo $this->_lexer->token;
			echo "id:\n" . $this->_lexer->id;
			exit(1);
			//$this->_lexer->getsym();
		}
	}
	
	public function getPureText()
	{
		return str_replace(" ", " ", $this->_pureText);
	}
}