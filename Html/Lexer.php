<?php
class Html_Lexer
{
	protected $sym;
	protected $num;
	protected $id = "";
	protected $token;
	protected $text;
	protected $current = 0;	// 文本内容指针
	protected $symtable;
	
	public function __construct()
	{
		$this->symtable = new Html_Symbol();
		$this->symtable->init();
	}
	
	public function getsym()
	{
		$this->id = "";
		if ($this->text[$this->current] == '<') {
			//$this->id = $this->text[$this->current];
			if ($this->text[$this->current + 1] == '/') {
				while ($this->text[$this->current] != '>') {
					$this->id .= $this->text[$this->current++];
				}
				$this->id .= $this->text[$this->current++];
				$symIndex = $this->symtable->lookup($this->id);
				$this->token = $this->symtable->symtable[$symIndex]["token"];
			} else {
				while ($this->text[$this->current] != '>') {
					$this->id .= $this->text[$this->current++];
				}
				$this->id .= $this->text[$this->current++];
				$symIndex = $this->symtable->lookup($this->id);
				// echo $symIndex;
				$this->token = $this->symtable->symtable[$symIndex]["token"];
			}
		} elseif($this->text[$this->current] == "\0") {
			$this->token = DONE;
		} else {
			// html文本结点
			while (isset($this->text[$this->current]) && $this->text[$this->current] != '<' && $this->text[$this->current] != "\0") {
				$this->id .= $this->text[$this->current++];
			}
			$this->token = HTML_TEXT;
		}
	}
	
	public function __get($name)
	{
		return $this->$name;
	}
	
	public function __set($name, $value)
	{
		$this->$name = $value;
	}
}