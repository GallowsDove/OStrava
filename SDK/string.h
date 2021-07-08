#ifndef _STRING_H_
#define _STRING_H_

void inline memcpy(void* dst, const void* src, int i)
{
	/* needed for ptr arithmetics */
	register char* d = (char*)dst;
	register char* s = (char*)src;
	
	/* not reasonable use */
	if (d == s || i == 0)
		return;
	
	/* overlap? */
	if (d - s < (unsigned long)i)
	{
		/* we will copy from backwards */
		d += i - 1;
		s += i - 1;
		
		/* is it worth to optimize? */
		if (i < 8)
		{
			/* no, buffers too small, just copy */
			while(i--)
				*d-- = *s--;
		}
		else
		{
			/* yes, align now */
			while(!((long)d & 3))
			{
				i--;
				*d-- = *s--;
			}
			
			d -= 3;
			s -= 3;
			
			/* cast to long ptrs */
			register long* ld = (long*)d;
			register long* ls = (long*)s;
			
			/* num of dwords to copy */
			int n = i >> 2;
			
			/* copy dwords */
			while(n--)
				*ld-- = *ls--;
			
			/* remaining bytes to copy */
			i &= 3;
			d = (char*)ld + 3;
			s = (char*)ls + 3;
			
			/* copy byte by byte */
			while(i--)
				*d-- = *s--;
		}
	}
	else
	{
		/* is it worth to optimize? */
		if (i < 8)
		{
			/* for small buffs just copy byte by byte */
			while(i--)
				*d++ = *s++;
		}
		else
		{
			/* align */
			while((long)d & 3)
			{
				i--;
				*d++ = *s++;
			}
			
			/* long = 4 bytes (dword) */
			register long* ld = (long*)d;
			register long* ls = (long*)s;
			
			/* num of dwords to copy */
			int n = i >> 2;
			
			/* copy dwords */
			while(n--)
				*ld++ = *ls++;
			
			/* align to end */
			d = (char*)ld;
			s = (char*)ls;
			i &= 3;
			
			/* copy byte by byte */
			while(i--)
				*d++ = *s++;
		}
	}
}

#define memmove memcpy

void inline memset(void* dst, char c, int i)
{
	register char* d = (char*)dst;
	
	/* worth optimizing? */
	if (i < 8)
	{
		/* set mem */
		while(i--)
			*d++ = c;
	}
	else
	{
		/* align */
		while((long)d & 3)
		{
			i--;
			*d++ = c;
		}
		
		register long* ld = (long*)d;
		register long lc = c;
		
		/* make dword from c */
		lc = lc | (lc << 8) | (lc << 16) | (lc << 24);
		
		/* num of dwords to set */
		int n = i >> 2;
		
		/* set dwords */
		while(n--)
			*ld++ = lc;
		
		/* align to end */
		d = (char*)ld;
		i &= 3;
		
		/* set the end of buff */
		while(i--)
			*d++ = c;
	}
}

#define memfill memset
#define memreset(ptr, n) memset((ptr), (char)(0), (n))

int inline memcmp(const void* p1, const void* p2, int i)
{
	/* cast to usable type */
	register const unsigned char* s1 = (const unsigned char*)p1;
	register const unsigned char* s2 = (const unsigned char*)p2;
	
	/* compare memblocks */
	while(i--)
		if (*s1++ != *s2++)
			return *--s1 < *--s2 ? -1 : 1;
	
	return 0;
}

int inline strsize(const char* s)
{
	register int i = 0;

	/* search for null-termination */
	while (s[i++]);

	return i;
}

#define strlen(s) (strsize((s)) - 1)

#define strcpy(d, s) memcpy((d), (s), strsize((s)))

#define strcmp(s1, s2) memcmp((s1), (s2), strsize((s2)))

#define strcat(d, s) memcpy(&(d)[strlen((d))], (s), strsize((s)))

char* strstr(const char* s1, const char* s2)
{
	/* do not concat this strlen with memcmp because it would be called in a loop but return value is always const = optimalization */
	const int l = strlen(s2);

	/* for each char in s1 */
	while (*s1)
	{
		/* if pattern found, return its addr */
		if (!memcmp(s1, s2, l))
			return (char*)s1;

		s1++;
	}

	return 0;
}

#endif
