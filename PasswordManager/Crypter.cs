using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PasswordManager
{
	class Crypter
	{
		private char[] key;
		private Int32 seed;

		public Crypter(string keyString)
		{
			key = keyString.ToCharArray();

			int size = key.Length;
			int loop = (size < 4) ? size : 4;
			for (int i = 0; i < loop; i++)
			{
				seed <<= 10;
				seed += (Int32)(key[i]);
			}
			Int32 xorSeed = 0x4A160C2E;
			loop = (size < 8) ? size : 8;
			for (int i = 4; i < loop; i++)
			{
				xorSeed <<= 10;
				xorSeed += (Int32)(key[i]);
			}
			seed ^= xorSeed;
		}

		public string encrypt(string text)
		{
			Random r = new Random(seed);
			char[] str = xorString(text.ToCharArray());
			
			for (uint i = 0; i < str.Length; i++)
			{
				str[i] = (char)(str[i] + (r.Next() % 0xFFFF));
			}

			return new string(str);
		}
		public string decrypt(string text)
		{
			Random r = new Random(seed);
			char[] str = text.ToCharArray();

			for (uint i = 0; i < str.Length; i++)
			{
				str[i] = (char)(str[i] - (r.Next() % 0xFFFF));
			}

			return new string(xorString(str));
		}

		private char[] xorString(char[] str)
		{ 
			for (uint i = 0; i < str.Length; i++)
				str[i] ^= key[i % key.Length];

			return str;
		}
	}
}
