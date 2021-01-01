#include "unicodeUtf8.h"
#include "common.h"

/*详细的ascii码编码规则见书P60*/

//返回value按照utf8编码后的字节数
uint32_t getByteNumOfEncodeUtf8(int value) {
	ASSERT(value > 0, "can't encode negative value!\n");  //只能对正数编码

	//单个ASCII码字符需要1个字节
	if (value <= 0x7f) {
		return 1;
	}
	//此范围需要2字节
	if (value <= 0x7ff) {
		return 2;
	}
	//此范围需要3字节
	if (value <= 0xffff) {
		return 3;
	}
	//此范围需要4字节
	if (value <= 0x10ffff) {
		return 4;
	}
	//超过范围返回0
	return 0;
}

//把value编码为utf8后写入缓冲区buf，返回写入的字节数
//详细的解释在书p65
uint8_t encodeUtf8(uint8_t* buf, int value) {
	ASSERT(value > 0, "can't encode negative value!\n");  //只能对正数编码

	//按照大端字节序写入缓冲区(低地址对应高数据位)
	if (value <= 0x7f) {
		*buf = value & 0x7f;   //讲value存入内存中
		return 1;
	}
	else if (value <= 0x7ff) {
		//这种情况下utf8格式为110xxxxx 10xxxxxx
		//先在低地址处写入高字节110xxxxx
		//11000000 | ((value & 0111 1100 0000)>>6)
		*buf++ = 0xc0 | ((value & 0x7c0) >> 6);
		//再在高地址处写入低字节10xxxxxx
		//1000 0000 | (value & 0011 1111)
		*buf = 0x80 | (value & 0x3f);
		return 2;
	}
	else if (value <= 0xffff) {
		//这种情况下utf8格式为1110xxxx 10xxxxxx 10xxxxxx
		//先在低地址写入高字节1110xxxx
		*buf++ = 0xe0 | ((value & 0xf0000) >> 12);
		//然后在中地址写入中字节
		*buf++ = 0x80 | ((value & 0xfc0) >> 6);
		//最后在高地址写入低字节
		*buf = 0x80 | (value & 0x3f);
		return 3;
	}
	else if (value <= 0x10ffff) {
		//在这种情况下utf8格式为11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//先在低地址写入高字节11110xxx
		*buf++ = 0xf0 | ((value & 0x1c0000) >> 18);
		//再在第二段地址写入第二段字节10xxxxxx
		*buf++ = 0x80 | ((value & 0x3f00) >> 12);
		//然后在第三段地址写入第三段字节10xxxxxx
		*buf++ = 0x80 | ((value & 0xfc0) >> 6);
		//最后在高地址写入低字节10xxxxxx
		*buf = 0x80 | (value & 0x3f);
		return 4;
	}

	NOTREACHED();  //如果程序运行到这里就是错了
	return 0;
}

//返回解码utf8的字节数
uint32_t getByteNumOfDecodeUtf8(uint8_t byte) {
	//byte应该是utf8的最高1字节，如果指向了utf8编码后面低字节部分则返回0
	if ((byte & 0xc0) == 0x80) return 0;  //1000 0000
	if ((byte & 0xf8) == 0xf0) return 4;  //1111 0000
	if ((byte & 0xf0) == 0xf0) return 3;  //1110 0000
	if ((byte & 0xe0) == 0xc0) return 2;  //1100 0000
	return 1;          //ASCII码
}

//解码以bytePtr为起始地址的utf8序列，其最大长度为length，若不是utf8序列就返回-1
int decodeUtf8(const uint8_t* bytePtr, uint32_t length) {
	//若是1字节的ascii码： 0xxxxxxx
	if (*bytePtr <= 0x7f) return *bytePtr;

	int value;
	uint32_t remainingBytes;

	//先读取高1字节
	//根据高1字节的高n位判断相应字节数的utf8编码
	if ((*bytePtr & 0xe0) == 0xc0) {
		//若是2字节的utf8
		value = *bytePtr & 0x1f; //记录后面的5位有效位
		remainingBytes = 1;
	}
	else if ((*bytePtr & 0xf0) == 0xe0) {
		//若是3字节的utf8
		value = *bytePtr & 0x0f; //记录后面的4位有效位
		remainingBytes = 2;
	}
	else if ((*bytePtr & 0xf8) == 0xf0) {
		//若是4字节的utf8
		value = *bytePtr & 0x07;
		remainingBytes = 3;
	}
	else { return -1; } //非法编码

	//如果utf8被斩断了就不再读下去了
	if (remainingBytes > length - 1) { return -1; }

	//再读取低字节中的数据
	while (remainingBytes > 0) {
		bytePtr++;
		remainingBytes--;
		//高两位必须是10
		if ((*bytePtr & 0xc0) != 0x80) { return -1; }
		//从次高字节往低字节读
		value = value << 6 | (*bytePtr & 0x3f); //value左移6为写入6位有效位
	}
	return value;   //返回解码得到的value值
}