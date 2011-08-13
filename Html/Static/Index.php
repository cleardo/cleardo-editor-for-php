<?php
class Html_Static_Index 
{
	protected $_config;
	
	public function __construct()
	{
		$this->_config = Globals::getConfig();
	}
	
	public function home()
	{
		$indexTmpl = new Templater();
		$indexTmpl->websiteUrl = $this->_config->website->url;
		$indexTmpl->blogUrl = $this->_config->website->blog->url;
		$indexTmpl->isDevel = $this->_config->dev;
		
		$htmlParser = new Html_Parser();
		
		$blog = new Blog();	// 博客对象
		$blog->getLast();	// 取得最后一篇博客
		$indexTmpl->topBlog = $blog;
		$topBlogContent = $blog->getBody();
		
		$htmlParser->setContent($topBlogContent);
		$htmlParser->parse();	// 进行html解析
		$indexTmpl->topBlogBody = $htmlParser->getPureText();

		//$indexTmpl->topBlogDate = date("Y年m月d日", $blog->getPublishedTime());
		$blogIdList = $blog->getIndexBlogs();
		$blogList = array();
		$blogIndex = 0;
		foreach ($blogIdList as $key => $value)
		{
			$blog = new Blog($value);
			$htmlParser = new Html_Parser();
			$htmlParser->setContent($blog->getBody());
			$htmlParser->parse();
			$blogList[$blogIndex]["blogtext"] = $htmlParser->getPureText();
			$blogList[$blogIndex]["title"] = $blog->getTitle();
			$blogList[$blogIndex]["date"] = $blog->getModifiedDate();
			$blogList[$blogIndex]["url"] = $blog->getUrl();
			$blogList[$blogIndex]["time"] = $blog->getModifiedTime();
			$blogIndex++;
		}
		
		$indexTmpl->indexBlogs = $blogList;
		
		$tweet = new Tweet();
		$tweetList = $tweet->getHomeTweet();
		$indexTmpl->tweetList = $tweetList;
		
		$blogCount = $blog->getblogyear();
		// Zend_Debug::dump($blogCount);
		$indexTmpl->blogCount = $blogCount;
		
		$content = $indexTmpl->render('html/index/index.tpl');
		$fp = fopen($this->_config->paths->htdoc . "/index.shtml", "w");
		fwrite($fp, $content);
	}
}