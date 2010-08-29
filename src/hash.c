/* assumes sizeof(unsigned)==4 */

#define rot(x, k) (((x)<<(k)) ^ ((x)>>(32-(k))))

/* mix 3 32 bit values reversibly */
#define mix(a, b, c) { \
	a -= c;  a ^= rot(c, 4);  c += b; \
	b -= a;  b ^= rot(a, 6);  a += c; \
	c -= b;  c ^= rot(b, 8);  b += a; \
	a -= c;  a ^= rot(c, 16);  c += b; \
	b -= a;  b ^= rot(a, 19);  a += c; \
	c -= b;  c ^= rot(b, 4);  b += a; \
}

/* final mixing of 3 32-bit values (a, b, c) into c */
#define final(a, b, c) { \
	c ^= b; c -= rot(b, 14); \
	a ^= c; a -= rot(c, 11); \
	b ^= a; b -= rot(a, 25); \
	c ^= b; c -= rot(b, 16); \
	a ^= c; a -= rot(c, 4); \
	b ^= a; b -= rot(a, 14); \
	c ^= b; c -= rot(b, 24); \
}

enum { seed = 0x9e3779b9 };

/* bob jenkins: lookup 3 */
unsigned jenhash(const void *key, unsigned len) {
	unsigned a, b, c;
	const unsigned *k = (const unsigned *)key;
	
	a = b = c = seed;
	while (size > 12) {
		a += *k++;
		b += *k++;
		c += *k++;
		mix(a, b, c)
		size -= 12;
	}
	switch (size) {
		case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
		case 11: c+=k[2]&0xffffff; b+=k[1]; a+=k[0]; break;
		case 10: c+=k[2]&0xffff; b+=k[1]; a+=k[0]; break;
		case 9 : c+=k[2]&0xff; b+=k[1]; a+=k[0]; break;
		case 8 : b+=k[1]; a+=k[0]; break;
		case 7 : b+=k[1]&0xffffff; a+=k[0]; break;
		case 6 : b+=k[1]&0xffff; a+=k[0]; break;
		case 5 : b+=k[1]&0xff; a+=k[0]; break;
		case 4 : a+=k[0]; break;
		case 3 : a+=k[0]&0xffffff; break;
		case 2 : a+=k[0]&0xffff; break;
		case 1 : a+=k[0]&0xff; break;
	}
	final(a, b, c)
	return c;
}

unsigned jenhash32(unsigned k) {
	unsigned a, b, c;
	
	a = b = c = seed;
	a += k;
	final(a, b, c)
	return c;
}

/* austin appleby: murmur 2 */
unsigned murmurhash(const void *key, unsigned len) {
	enum { m = 0x5bd1e995, r = 24 };
	unsigned h = seed ^ len;
	const unsigned char *data = (const unsigned char *)key;

	while(len >= 4) {
		unsigned k = *(unsigned *)data;
		k *= m;
		k ^= k >> r;
		k *= m;
		h *= m;
		h ^= k;
		data += 4;
		len -= 4;
	}
	switch(len) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

unsigned murmurhash32(unsigned k) {
	return murmurhash(&k, 4);
}

/* simple hash for aligned pointers */
unsigned ptrhash(const void *k) {
	unsigned long n = (unsigned long)k;
	return (n >> 2) ^ (n >> 12);
}

/* simple string hash */
unsigned strhash(const char *key) {
	unsigned char *p = (unsigned char *)key;
	unsigned h = seed;

	while (*p)
		h += (h << 2) + *p++;
	return h;
}

