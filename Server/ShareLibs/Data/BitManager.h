#pragma once 


namespace ShareLib{namespace Data{




	template<typename Type> //Accept char short long longlong 
	class CBitSysType{
		volatile Type m_bitmask;


	public:
		inline CBitSysType():m_bitmask(0){};
		inline CBitSysType(Type mask):m_bitmask(mask){};

		inline Type bitmask(){return m_bitmask;};
		inline void bitmask(Type mask){m_bitmask=mask;}
		template<int mask>inline Type get_bit(){return m_bitmask&mask;}
		template<int mask>inline void set_bit(){m_bitmask|=mask;}
		template<int mask>inline void clr_bit(){m_bitmask&=~mask;}

		inline Type is_marked(Type mask){return m_bitmask&mask;}
		inline void mark(Type mask){m_bitmask|=mask;}
		inline void unmark(Type mask){m_bitmask&=~mask;}

		Type Value(){return m_bitmask;};
		void Value(Type value){m_bitmask=value;}
	};


}}