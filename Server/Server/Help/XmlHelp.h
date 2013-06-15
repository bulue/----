#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"

template<typename Ty>
class CXmlHelp
{
	TiXmlDocument m_Document;
public:
	void Load(Ty* const obj,char* _filename)
	{
		TiXmlElement * root;
		if (!m_Document.LoadFile(_filename) ||(root = m_Document.RootElement()) == NULL)
		{
			return ;
		}
		*root >> static_cast<Ty&>(*obj);
	}
};