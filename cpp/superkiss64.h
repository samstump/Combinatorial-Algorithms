// superkiss64.h

// This is one of the generators from George Marsaglia (http://mathforum.org/kb/message.jspa?messageID=6917990), 2009.
// It carries some state, but is very fast.  Passes every known randomness test to date.

class superkiss64 {
	public:
		superkiss64()
		{
			carry = 36243678541LL;
			xcng = 12367890123456LL;
			xs = 521288629546311LL;
			init();
		}
		
		superkiss64(	unsigned long long _xs,		// 64 significant bits
						unsigned long long _xcng,	// 64 significant bits
						unsigned long long _carry)	// 41 significant bits (0x1FFFFFFFFFF)
		{
			carry = _carry;
			xcng = _xcng;
			xs = _xs;
			init();
		}
		
		
		unsigned long long rand()
		{
			return SUPR() + CNG() + XS();
		}
		
		double rand01()
		{
			// double has 53 bits of precision.  
			// First constant is a bitmask of the 53 LSBs, Second constant is 2^53
			return (rand() & 0x1FFFFFFFFFFFFF) / 9007199254740992.0;
		}
		
	private:
	
		unsigned long long SUPR()
		{
			return indx < sizeQ ? Q[indx++] : refill();
		}
		
		unsigned long long CNG()
		{
			xcng = 6906969069LL * xcng + 123;
			return xcng;
		}
		
		unsigned long long XS()
		{
			xs^=xs<<13;
			xs^=xs>>17;
			return xs^=xs<<43;
		}
	
		void init()
		{
			indx = sizeQ;
			
			for(int i=0; i<sizeQ; i++) {
				Q[i]=CNG() + XS();
			}
		}
		
		unsigned long long refill()
		{
			for(int i=0; i<sizeQ; i++) {
				unsigned long long h = (carry & 1);
				unsigned long long z = ((Q[i]<<41)>>1) + ((Q[i]<<39)>>1) + (carry>>1);
				carry = (Q[i]>>23) + (Q[i]>>25) + (z>>63);
				Q[i] = ~((z<<1)+h);
			}
			indx = 1;
			return Q[0];
		}
		
		const static int sizeQ = 20632;

		unsigned long long Q[sizeQ];
		unsigned long long carry;
		unsigned long long xcng;
		unsigned long long xs;
		unsigned long long indx;	
};
