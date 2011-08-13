<?php
class Html_Symbol
{
	public $symtable = array();
	public $complete = array();
	
	public function init()
	{
		$this->symtable[0]["token"] = HTML_UNDEFINED;
		$this->symtable[0]["pattern"] = "<.*?>";
		$this->symtable[0]["value"] = "";
		$this->symtable[1]["token"] = HTML_BEGIN_P;
		$this->symtable[1]["pattern"] = "<p.*?>";
		$this->symtable[1]["value"] = "<p>";
		$this->symtable[2]["token"] = HTML_END_P;
		$this->symtable[2]["pattern"] = "</p>";
		$this->symtable[2]["value"] = "</p>";
		$this->symtable[3]["token"] = HTML_BEGIN_DIV;
		$this->symtable[3]["pattern"] = "<div.*?>";
		$this->symtable[3]["value"] = "<div>";
		$this->symtable[4]["token"] = HTML_END_DIV;
		$this->symtable[4]["pattern"] = "</div>";
		$this->symtable[4]["value"] = "</div>";
		$this->symtable[5]["token"] = HTML_BEGIN_PRE;
		$this->symtable[5]["pattern"] = "<pre.*?>";
		$this->symtable[5]["value"] = "<pre>";
		$this->symtable[6]["token"] = HTML_END_PRE;
		$this->symtable[6]["pattern"] = "</pre>";
		$this->symtable[6]["value"] = "</pre>";
		$this->symtable[7]["token"] = HTML_IMG;
		$this->symtable[7]["pattern"] = "<img.*?>";
		$this->symtable[7]["value"] = "<img>";
		$this->symtable[8]["token"] = HTML_BEGIN_HREF;
		$this->symtable[8]["pattern"] = "<a\\s+href.*?>";
		$this->symtable[8]["value"] = "<a>";
		$this->symtable[9]["token"] = HTML_END_HREF;
		$this->symtable[9]["pattern"] = "</a>";
		$this->symtable[9]["value"] = "</a>";
		$this->symtable[10]["token"] = HTML_BR;
		$this->symtable[10]["pattern"] = "<br\\s+/>";
		$this->symtable[10]["value"] = "<br/>";
		
		$this->complete[HTML_END_P] = "</p>";
	}
	
	public function lookup($symbol)
	{
		$this->symtable[0]["value"] = $symbol;
		$count = count($this->symtable) - 1;
		// echo $count;
		while ($count) {
			if ($this->symtable[$count]["value"] == $symbol || preg_match($this->symtable[$count]["pattern"], $symbol)) {
				return $count;
			} else {
				$count--;
			}
		}
		return $count;
	}
	
	public function insert()
	{
		
	}
}