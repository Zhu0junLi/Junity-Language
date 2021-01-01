#include "unicodeUtf8.h"
#include "common.h"

/*��ϸ��ascii�����������P60*/

//����value����utf8�������ֽ���
uint32_t getByteNumOfEncodeUtf8(int value) {
	ASSERT(value > 0, "can't encode negative value!\n");  //ֻ�ܶ���������

	//����ASCII���ַ���Ҫ1���ֽ�
	if (value <= 0x7f) {
		return 1;
	}
	//�˷�Χ��Ҫ2�ֽ�
	if (value <= 0x7ff) {
		return 2;
	}
	//�˷�Χ��Ҫ3�ֽ�
	if (value <= 0xffff) {
		return 3;
	}
	//�˷�Χ��Ҫ4�ֽ�
	if (value <= 0x10ffff) {
		return 4;
	}
	//������Χ����0
	return 0;
}

//��value����Ϊutf8��д�뻺����buf������д����ֽ���
//��ϸ�Ľ�������p65
uint8_t encodeUtf8(uint8_t* buf, int value) {
	ASSERT(value > 0, "can't encode negative value!\n");  //ֻ�ܶ���������

	//���մ���ֽ���д�뻺����(�͵�ַ��Ӧ������λ)
	if (value <= 0x7f) {
		*buf = value & 0x7f;   //��value�����ڴ���
		return 1;
	}
	else if (value <= 0x7ff) {
		//���������utf8��ʽΪ110xxxxx 10xxxxxx
		//���ڵ͵�ַ��д����ֽ�110xxxxx
		//11000000 | ((value & 0111 1100 0000)>>6)
		*buf++ = 0xc0 | ((value & 0x7c0) >> 6);
		//���ڸߵ�ַ��д����ֽ�10xxxxxx
		//1000 0000 | (value & 0011 1111)
		*buf = 0x80 | (value & 0x3f);
		return 2;
	}
	else if (value <= 0xffff) {
		//���������utf8��ʽΪ1110xxxx 10xxxxxx 10xxxxxx
		//���ڵ͵�ַд����ֽ�1110xxxx
		*buf++ = 0xe0 | ((value & 0xf0000) >> 12);
		//Ȼ�����е�ַд�����ֽ�
		*buf++ = 0x80 | ((value & 0xfc0) >> 6);
		//����ڸߵ�ַд����ֽ�
		*buf = 0x80 | (value & 0x3f);
		return 3;
	}
	else if (value <= 0x10ffff) {
		//�����������utf8��ʽΪ11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//���ڵ͵�ַд����ֽ�11110xxx
		*buf++ = 0xf0 | ((value & 0x1c0000) >> 18);
		//���ڵڶ��ε�ַд��ڶ����ֽ�10xxxxxx
		*buf++ = 0x80 | ((value & 0x3f00) >> 12);
		//Ȼ���ڵ����ε�ַд��������ֽ�10xxxxxx
		*buf++ = 0x80 | ((value & 0xfc0) >> 6);
		//����ڸߵ�ַд����ֽ�10xxxxxx
		*buf = 0x80 | (value & 0x3f);
		return 4;
	}

	NOTREACHED();  //����������е�������Ǵ���
	return 0;
}

//���ؽ���utf8���ֽ���
uint32_t getByteNumOfDecodeUtf8(uint8_t byte) {
	//byteӦ����utf8�����1�ֽڣ����ָ����utf8���������ֽڲ����򷵻�0
	if ((byte & 0xc0) == 0x80) return 0;  //1000 0000
	if ((byte & 0xf8) == 0xf0) return 4;  //1111 0000
	if ((byte & 0xf0) == 0xf0) return 3;  //1110 0000
	if ((byte & 0xe0) == 0xc0) return 2;  //1100 0000
	return 1;          //ASCII��
}

//������bytePtrΪ��ʼ��ַ��utf8���У�����󳤶�Ϊlength��������utf8���оͷ���-1
int decodeUtf8(const uint8_t* bytePtr, uint32_t length) {
	//����1�ֽڵ�ascii�룺 0xxxxxxx
	if (*bytePtr <= 0x7f) return *bytePtr;

	int value;
	uint32_t remainingBytes;

	//�ȶ�ȡ��1�ֽ�
	//���ݸ�1�ֽڵĸ�nλ�ж���Ӧ�ֽ�����utf8����
	if ((*bytePtr & 0xe0) == 0xc0) {
		//����2�ֽڵ�utf8
		value = *bytePtr & 0x1f; //��¼�����5λ��Чλ
		remainingBytes = 1;
	}
	else if ((*bytePtr & 0xf0) == 0xe0) {
		//����3�ֽڵ�utf8
		value = *bytePtr & 0x0f; //��¼�����4λ��Чλ
		remainingBytes = 2;
	}
	else if ((*bytePtr & 0xf8) == 0xf0) {
		//����4�ֽڵ�utf8
		value = *bytePtr & 0x07;
		remainingBytes = 3;
	}
	else { return -1; } //�Ƿ�����

	//���utf8��ն���˾Ͳ��ٶ���ȥ��
	if (remainingBytes > length - 1) { return -1; }

	//�ٶ�ȡ���ֽ��е�����
	while (remainingBytes > 0) {
		bytePtr++;
		remainingBytes--;
		//����λ������10
		if ((*bytePtr & 0xc0) != 0x80) { return -1; }
		//�Ӵθ��ֽ������ֽڶ�
		value = value << 6 | (*bytePtr & 0x3f); //value����6Ϊд��6λ��Чλ
	}
	return value;   //���ؽ���õ���valueֵ
}