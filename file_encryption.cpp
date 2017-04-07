#include<time.h>
#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<fstream>
using namespace std;
struct byte_storage
{
	int value;
	struct byte_storage *next;
};
int s0[16] = {3,13,0,7,12,6,2,5,14,1,15,4,8,11,9,10};
int s1[16] = {5,0,10,13,2,7,11,12,3,6,4,14,9,1,15,8};
int countSetBits(int n)
{
	
	
  unsigned int c; // the total bits set in n
  for (c = 0; n; n >>= 1)
  {
    c += n & 1;
  }
  return c;
}
int map_enc(int n, int box)
{
	if(box == 0)
		return s0[n];
	else
		return s1[n];
}
int map_dec(int n,int box)
{
	int i = 0, j = 0;
	if(box == 0)
	{
		for(i=0;i<16;i++)
		{
			if(n==s0[i])
			{
				j=i;
				break;
			}
		}
		
	}
	else
	{
		for(i=0;i<16;i++)
		{
			if(n==s1[i])
			{
				j=i;
				break;
			}
		}
	}
	return j;
}
long dec2bin(int n)
{
	
	int r;
	long bin =0, i =1;
	while(n!=0)
	{
		r = n%2;
		n = n/2;
		bin += r*i;
		i *= 10;
	}
	return bin;
}
int bin2dec(long n)
{
	int dec = 0;
	int b = 1, r = 0;
	while(n>0)
	{
		r = n%10;
		dec += r*b;
		n /= 10;
		b *= 2;
	}
	return dec;
}

int encrypt(char key[2], char msg[2])
{
	
	int temp[2];
	int temp1[2];
	//long key_2bytes = 0;
	long bin_merged_2bytes = 0;
	temp[0]=int(key[0]);
	temp[1] = int(key[1]);
	
	
	temp1[0] = int (msg[0]);
	temp1[1] = int (msg[1]);
	for(int i = 0;i<2;i++)
	{
		if(temp[i]<0)
		temp[i]+=256;
		if(temp1[i]<0)
		temp1[i]+=256;
	}
	//printf("Key = %d %d\n",temp[0],temp[1]);
	//printf("Message = %d %d\n",temp1[0],temp1[1]);
	//key_2bytes = temp[0]*1000;
	//key_2bytes += temp[1];
	//printf("Merged key = %ld\n",key_2bytes);
	long bin_msg0_byte = dec2bin(temp1[0]);
	long bin_msg1_byte = dec2bin(temp1[1]);
	//printf("Message bytes = %ld %ld\n",bin_msg0_byte,bin_msg1_byte);
	long bin_xor0_byte = dec2bin(bin2dec(bin_msg0_byte)^temp[0]);
	long bin_xor1_byte = dec2bin(bin2dec(bin_msg1_byte)^temp[1]);
	//printf("Key bytes = %ld %ld\n",dec2bin(temp[0]),dec2bin(temp[1]));
	//printf("Bytes after XOR = %ld %ld\n",bin_xor0_byte,bin_xor1_byte);
	long bin_msg0_nibble[2] = {0,0};
	long bin_msg1_nibble[2] = {0,0};
	bin_msg0_nibble[0] = bin_xor0_byte/10000;
	bin_msg0_nibble[1] = bin_xor0_byte%10000;
	bin_msg1_nibble[0] = bin_xor1_byte/10000;
	bin_msg1_nibble[1] = bin_xor1_byte%10000;
	//printf("Message0 nibbles = %ld %ld\tMessage1 nibbles = %ld %ld\n",bin_msg0_nibble[0],bin_msg0_nibble[1],bin_msg1_nibble[0],bin_msg1_nibble[1]);
	long s_out0[2] = {dec2bin(map_enc(bin2dec(bin_msg0_nibble[0]),0)),dec2bin(map_enc(bin2dec(bin_msg0_nibble[1]),1))};
	long s_out1[2] = {dec2bin(map_enc(bin2dec(bin_msg1_nibble[0]),0)),dec2bin(map_enc(bin2dec(bin_msg1_nibble[1]),1))};
	//printf("S box out nibbles = %ld %ld %ld %ld\n",s_out0[0],s_out0[1],s_out1[0],s_out1[1]);
	long bin_merged0_byte = s_out0[0]*10000;
	bin_merged0_byte += s_out0[1];
	long bin_merged1_byte = s_out1[0]*10000;
	bin_merged1_byte += s_out1[1];
	//printf("Merged bytes = %ld %ld\n",bin_merged0_byte,bin_merged1_byte);
	long bin_xor2_byte = dec2bin(temp[0] ^ bin2dec(bin_merged0_byte));
	long bin_xor3_byte = dec2bin(temp[1] ^ bin2dec(bin_merged1_byte));
	//printf("Second XOR bytes = %ld %ld\n",bin_xor2_byte,bin_xor3_byte);
	int cipher = 0;
	int temp2 = bin2dec(bin_xor2_byte)*1000;
	temp2 += bin2dec(bin_xor3_byte);
	cipher = temp2;
	//printf("bin2dec of byte0 = %d\n",temp2);
	//cipher += bin2dec(bin_merged1_byte);
	//printf("Final function output = %d\n",cipher);
	//int cipher = bin2dec(bin_merged_2bytes)^key_2bytes;
	return(cipher);
}
int decrypt(char key[2], char cipher[2])
{
	//printf("Decryption start\n");
	//long key_2bytes = 0;
	int temp[2], temp1[2];
	temp[0]=int(key[0]);
	temp[1] = int(key[1]);
	
	temp1[0] = int (cipher[0]);
	temp1[1] = int (cipher[1]);
	for(int i = 0;i<2;i++)
	{
		if(temp[i]<0)
		temp[i]+=256;
		if(temp1[i]<0)
		temp1[i]+=256;
	}
	//printf("Key = %d %d\n",temp[0],temp[1]);
	//printf("Message = %d %d\n",temp1[0],temp1[1]);
	//key_2bytes = temp[0]*1000;
	//key_2bytes += temp[1];
	//printf("Merged key = %ld\n",key_2bytes);
	long bin_msg0_byte = dec2bin(temp1[0]);
	long bin_msg1_byte = dec2bin(temp1[1]);
	//printf("Message bytes = %ld %ld\n",bin_msg0_byte,bin_msg1_byte);
	long bin_xor0_byte = dec2bin(bin2dec(bin_msg0_byte)^temp[0]);
	long bin_xor1_byte = dec2bin(bin2dec(bin_msg1_byte)^temp[1]);
	//printf("Key bytes = %ld %ld\n",dec2bin(temp[0]),dec2bin(temp[1]));
	//printf("Bytes after XOR = %ld %ld\n",bin_xor0_byte,bin_xor1_byte);
	long bin_msg0_nibble[2] = {0,0};
	long bin_msg1_nibble[2] = {0,0};
	bin_msg0_nibble[0] = bin_xor0_byte/10000;
	bin_msg0_nibble[1] = bin_xor0_byte%10000;
	bin_msg1_nibble[0] = bin_xor1_byte/10000;
	bin_msg1_nibble[1] = bin_xor1_byte%10000;
	//printf("Message0 nibbles = %ld %ld\tMessage1 nibbles = %ld %ld\n",bin_msg0_nibble[0],bin_msg0_nibble[1],bin_msg1_nibble[0],bin_msg1_nibble[1]);
	long s_out0[2] = {dec2bin(map_dec(bin2dec(bin_msg0_nibble[0]),0)),dec2bin(map_dec(bin2dec(bin_msg0_nibble[1]),1))};
	long s_out1[2] = {dec2bin(map_dec(bin2dec(bin_msg1_nibble[0]),0)),dec2bin(map_dec(bin2dec(bin_msg1_nibble[1]),1))};
	//printf("S box out nibbles = %ld %ld %ld %ld\n",s_out0[0],s_out0[1],s_out1[0],s_out1[1]);
	long bin_merged0_byte = s_out0[0]*10000;
	bin_merged0_byte += s_out0[1];
	long bin_merged1_byte = s_out1[0]*10000;
	bin_merged1_byte += s_out1[1];
	//printf("Merged bytes = %ld %ld\n",bin_merged0_byte,bin_merged1_byte);
	long bin_xor2_byte = dec2bin(temp[0] ^ bin2dec(bin_merged0_byte));
	long bin_xor3_byte = dec2bin(temp[1] ^ bin2dec(bin_merged1_byte));
	//printf("Second XOR bytes = %ld %ld\n",bin_xor2_byte,bin_xor3_byte);
	int plain = 0;
	int temp2 = bin2dec(bin_xor2_byte)*1000;
	temp2 += bin2dec(bin_xor3_byte);
	plain = temp2;
	//printf("Final function output = %d\n",plain);
	return(plain);
	
}

int main()
{
	ofstream oFile;
	int i, len;
	char c;
	char key[4] = {'K','U','2','r'};
	char msg[4], cipher[4];
	char key1[2], key2[2], msg1[2], msg2[2];
	char cipher1[2], cipher2[2];
	for(i=0;i<2;i++)
	{
		key1[i] = key[i];
		key2[i] = key[i+2];
	}
	struct byte_storage *head_in, *temp_in, *tail_in;
	struct byte_storage *head_enc, *temp_enc, *tail_enc;
	struct byte_storage *head_dec, *temp_dec, *tail_dec;
	ifstream pFile;
	pFile.open ("demo.txt",ios::binary | ifstream::in);
	head_in = NULL;
	tail_in = NULL;
	temp_in = NULL;
	int rounds = 16;
	while(pFile.get(c))
	{
		if(head_in==NULL)
		{
			
			head_in = new struct byte_storage;
			head_in->value = int(c);
			head_in->next = NULL;
			tail_in = head_in;
		}
		else
		{
			temp_in = new struct byte_storage;
			temp_in->value = int(c);
			temp_in->next = NULL;
			tail_in->next = temp_in;
			tail_in = temp_in;
		}
	}
	pFile.close();
	temp_in = head_in;
	len = 0;
	while(temp_in!=NULL)
	{
		//printf("No: %d\tValue: %d\n",len,temp_in->value);
		len++;
		temp_in = temp_in->next;
	}
	printf("Length = %d\n",len);
	printf("Tail = %d\n",tail_in->value);
	int pad, block_count;
	if(len%4 == 0)
		pad = 0;
	else
		pad = 4 - (len%4);
	printf("Pad = %d\n",pad);
	
	if (pad!=0)
	{
		for(i=0;i<pad;i++)
		{
			temp_in = new struct byte_storage;
			temp_in->value = 0;
			temp_in->next = NULL;
			tail_in->next = temp_in;
			tail_in = temp_in;
		}
	}
	oFile.open("demo_out1.txt",ios::binary | ofstream::out);
	int temp_val = 0;
	temp_in = head_in;
	head_enc = NULL;
	tail_enc = NULL;
	char j;
	char *j_ptr = &j;
	while(temp_in!=NULL)
	{
		for(i=0;i<4;i++)
			msg[i] = '\0';
		for(block_count=0;block_count<4;block_count++)
		{
			temp_val = temp_in->value;
			//printf("%d ",temp_val);
			msg[block_count] = char(temp_in->value);
			//printf("%d ",int(msg[block_count]));
			temp_in = temp_in -> next;
		}
		//printf("\n");
		int ciph1 = 0, ciph10 = 0, ciph11 = 0;
		int ciph2 = 0, ciph20 = 0, ciph21 = 0;
		for(i=0;i<2;i++)
		{
			msg1[i] = msg[i];
			msg2[i] = msg[i+2];
		}
		for(i=0;i<rounds;i++)
		{
			//printf("MSG1 = %c%c\n",msg1[0],msg1[1]);
			//printf("MSG2 = %c%c\n",msg2[0],msg2[1]);
			//printf("Key1 = %c%c\n",key1[0],key1[1]);
			//printf("Key2 = %c%c\n",key2[0],key2[1]);
			ciph1 = encrypt(key1,msg1);
			ciph2 = encrypt(key2,msg2);
			ciph10 = ciph1/1000;
			ciph11 = ciph1%1000;
			ciph20 = ciph2/1000;
			ciph21 = ciph2%1000;
			/*ciph10 -= 128;
			ciph11 -= 128;
			ciph20 -= 128;
			ciph21 -= 128;*/
			cipher1[0] = char(ciph10);
			cipher1[1] = char(ciph11);
			cipher2[0] = char(ciph20);
			cipher2[1] = char(ciph21);

			msg1[0] = cipher2[0];
			msg1[1] = cipher2[1];
			msg2[0] = cipher1[0];
			msg2[1] = cipher1[1];
		}
	/*	
		int ciph0 = ciph/1000;
		int ciph1 = ciph%1000;
	*/
		//printf("ciph10 = %d ciph11 = %d\n",ciph10,ciph11);
		//printf("ciph20 = %d ciph21 = %d\n",ciph20, ciph21);
		//getch();
		cipher1[0] = char(ciph10);
		cipher1[1] = char(ciph11);
		cipher2[0] = char(ciph20);
		cipher2[1] = char(ciph21);
		for(i=0;i<2;i++)
		{
			cipher[i] = cipher1[i];
			cipher[i+2] = cipher2[i];
		}
		for(i=0;i<4;i++)
		{
			j = cipher[i];
			oFile.write(j_ptr,1);
			//printf("%d ",int(j));
			if(head_enc == NULL)
			{
				head_enc = new struct byte_storage;
				head_enc->value = int(j);
				head_enc->next = NULL;
				tail_enc = head_enc;
			}
			else
			{
				temp_enc = new struct byte_storage;
				temp_enc->value = int(j);
				temp_enc->next = NULL;
				tail_enc->next = temp_enc;
				tail_enc = temp_enc;
			}
		}
		//printf("\n");
	}
	oFile.close();
	pFile.open("demo_out1.txt",ios::binary | ifstream::in);
	head_enc = NULL;
	temp_enc = NULL;
	tail_enc = NULL;
	while(pFile.get(c))
	{
		if(head_enc==NULL)
		{
			
			head_enc = new struct byte_storage;
			head_enc->value = int(c);
			head_enc->next = NULL;
			tail_enc = head_enc;
		}
		else
		{
			temp_enc = new struct byte_storage;
			temp_enc->value = int(c);
			temp_enc->next = NULL;
			tail_enc->next = temp_enc;
			tail_enc = temp_enc;
		}
	}
	i = 0;
	len = 0;
	temp_enc = head_enc;
	while(temp_enc!=NULL)
	{
		//printf("%d ",temp_enc->value);
		len++;
		temp_enc = temp_enc->next;
	}
	pad = 0;
	if(len%4 == 0)
		pad = 0;
	else
		pad = 4 - (len%4);
	
	temp_enc = head_enc;
	head_dec = NULL;
	tail_dec = NULL;
	oFile.open("demo_dec.txt",ios::binary | ofstream::out);
	while(temp_enc!=NULL)
	{
		for(i=0;i<4;i++)
			msg[i] = '\0';
		for(block_count=0;block_count<4;block_count++)
		{
			msg[block_count] = char(temp_enc->value);
			temp_enc = temp_enc->next;
		}
		for(i=0;i<2;i++)
		{
			msg1[i] = msg[i];
			msg2[i] = msg[i+2];
		}
		int ciph1 = 0, ciph10 = 0, ciph11 = 0;
		int ciph2 = 0, ciph20 = 0, ciph21 = 0;
		for(i=0;i<rounds;i++)
		{
			ciph1 = decrypt(key1,msg1);
			ciph2 = decrypt(key2,msg2);
			ciph10 = ciph1/1000;
			ciph11 = ciph1%1000;
			ciph20 = ciph2/1000;
			ciph21 = ciph2%1000;
			/*ciph10 -= 128;
			ciph11 -= 128;
			ciph20 -= 128;
			ciph21 -= 128;*/
			cipher1[0] = char(ciph10);
			cipher1[1] = char(ciph11);
			cipher2[0] = char(ciph20);
			cipher2[1] = char(ciph21);

			msg1[0] = cipher2[0];
			msg1[1] = cipher2[1];
			msg2[0] = cipher1[0];
			msg2[1] = cipher1[1];
			//
			//getch();
		}
		//printf("Cipher values %d %d %d %d\n",ciph10,ciph11,ciph20,ciph21);
		//printf("Cipher Text = %c%c%c%c\n",char(ciph10),char(ciph11),char(ciph20),char(ciph21));
		cipher1[0] = char(ciph10);
		cipher1[1] = char(ciph11);
		cipher2[0] = char(ciph20);
		cipher2[1] = char(ciph21);
		for(i=0;i<2;i++)
		{
			cipher[i] = cipher1[i];
			cipher[i+2] = cipher2[i];
		}
		for(i=0;i<4;i++)
		{
			j = cipher[i];
			oFile.write(j_ptr,1);
			if(head_dec==NULL)
			{
				head_dec = new struct byte_storage;
				head_dec->value = int(j);
				head_dec->next = NULL;
				tail_dec = head_dec;
			}
			else
			{
				temp_dec = new struct byte_storage;
				temp_dec->value = int(j);
				temp_dec->next = NULL;
				tail_dec->next = temp_dec;
				tail_dec = temp_dec;
			}
			
		}
	}
	pFile.close();
	printf("Input heap\n");
	temp_in = head_in;
	i = 0;
	while(temp_in!=NULL)
	{
		printf("%d ",temp_in->value);
		i++;
		if(i%4 == 0)
			printf("\n");
		temp_in = temp_in->next;
	}
	printf("Encrypted heap\n");
	temp_enc = head_enc;
	i = 0;
	while(temp_enc != NULL)
	{
		printf("%d ",temp_enc->value);
		i++;
		if(i%4==0)
			printf("\n");
		temp_enc = temp_enc->next;
	}
	printf("Decrypted heap\n");
	temp_dec = head_dec;
	i = 0;
	while(temp_dec != NULL)
	{
		printf("%d ",temp_dec->value);
		i++;
		if(i%4==0)
			printf("\n");
		temp_dec = temp_dec->next;
	}
	
	printf("---------------------------------->");
	printf("\nBrute forcing\n");
	//BRUTE FORCE BLOCK
	char msg_first_block_cipher[4] = {'\0','\0','\0','\0'};
	char msg_first_block_plain[4]  = {'\0','\0','\0','\0'};
	char key_brute[4] = {'\0','\0','\0','\0'};
	
	
	
	//Loading the first cipher and plain text blocks
	temp_enc = head_enc;
	temp_dec = head_dec;
	i = 0;
	while(i<4)
	{
		msg_first_block_cipher[i] = char(temp_enc->value);
		msg_first_block_plain[i]  = char(temp_dec->value);
		temp_enc = temp_enc->next;
		temp_dec = temp_dec->next;
		i++;
	}
	printf("Cipher = %d %d %d %d\n",int(msg_first_block_cipher[0]),int(msg_first_block_cipher[1]),int(msg_first_block_cipher[2]),int(msg_first_block_cipher[3]));
	printf("Plain  = %d %d %d %d\n",int(msg_first_block_plain[0]),int(msg_first_block_plain[1]),int(msg_first_block_plain[2]),int(msg_first_block_plain[3]));
	int plain_brute1 = 0;
	int plain_brute2 = 0;
	int p10, p11, p20, p21;
	char plain_test[2] = {'\0','\0'};
	int ans1 = int(msg_first_block_plain[0]);
	ans1*=1000;
	ans1+=int(msg_first_block_plain[1]);
	int ans2 = int(msg_first_block_plain[2]);
	ans2*=1000;
	ans2+=int(msg_first_block_plain[3]);
	printf("Required answers = %d %d\n",ans1,ans2);
	long iterations = 0;
	int brute1 = 0, brute2= 0, brute3 = 0, brute4 = 0;
	int flag = 0;
	int ciph1 = 0, ciph2 = 0, ciph11 = 0, ciph10 = 0, ciph20 = 0, ciph21 = 0;
	int x;
	int prog = 0;
	clock_t start, end;
	double exec_time;
	start = clock();
	for(brute1 = 0;brute1<1;brute1++)
	{
		printf("\rBrute1 = %d",brute1);
		if(flag == 1)
			break;
		for(brute2 = 0;brute2<256;brute2++)
		{
			//printf("\nBrute2 = %d%%",brute2*100/256);
			if(flag == 1)
				break;
			for(brute3 = 0;brute3<256;brute3++)
			{
				//printf("\nBrute3 = %d%%",brute3*100/256);
				if(flag == 1)
					break;
				for(brute4 = 0;brute4<256;brute4++)
				{
					
					iterations++;
					//printf("\nBrute4 = %d%%",brute4*100/256);
					for(x=0;x<2;x++)
					{
						msg1[x] = msg_first_block_cipher[x];
						msg2[x] = msg_first_block_cipher[x+2];
						key1[0] = char(brute1);
						key1[1] = char(brute2);
						key2[0] = char(brute3);
						key2[1] = char(brute4);
					}
					//'{'K','U','2','r'}
					/*key1[0] = 'K';
					key1[1] = 'U';
					key2[0] = '2';
					key2[1] = 'r';
					printf("\n");
					printf("Input msg = %d %d %d %d\n",int(msg1[0]),int(msg1[1]),int(msg2[0]),int(msg2[1]));
					printf("Input key = %c%c%c%c \n",key1[0],key1[1],key2[0],key2[1]);*/
					/*for(i=0;i<rounds;i++)
					{
						
						ciph1 = decrypt(key1,msg2);
						ciph2 = decrypt(key2,msg2);
						ciph10 = ciph1/1000;
						ciph11 = ciph1%1000;
						ciph20 = ciph2/1000;
						ciph21 = ciph2%1000;
						cipher1[0] = char(ciph10);
						cipher1[1] = char(ciph11);
						cipher2[0] = char(ciph20);
						cipher2[1] = char(ciph21);

						msg1[0] = cipher2[0];
						msg1[1] = cipher2[1];
						msg2[0] = cipher1[0];
						msg2[1] = cipher1[1];
					}*/
					for(i=0;i<rounds;i++)
					{
						ciph1 = decrypt(key1,msg1);
						ciph2 = decrypt(key2,msg2);
						ciph10 = ciph1/1000;
						ciph11 = ciph1%1000;
						ciph20 = ciph2/1000;
						ciph21 = ciph2%1000;
						/*ciph10 -= 128;
						ciph11 -= 128;
						ciph20 -= 128;
						ciph21 -= 128;*/
						cipher1[0] = char(ciph10);
						cipher1[1] = char(ciph11);
						cipher2[0] = char(ciph20);
						cipher2[1] = char(ciph21);

						msg1[0] = cipher2[0];
						msg1[1] = cipher2[1];
						msg2[0] = cipher1[0];
						msg2[1] = cipher1[1];
						//
						//getch();
					}	
					//printf("Output msg = %d %d %d %d\n",int(cipher1[0]),int(cipher1[1]),int(cipher2[0]),int(cipher2[1]));
					//printf("Ciph1 = %d ciph2 = %d\n",ciph1,ciph2);
					//printf("diff = %d %d",ciph1-ans1,ciph2-ans2);
					//getch();
					if (ciph1==ans1 && ciph2==ans2)
					{
						flag = 1;
						break;
					}
				}
			}
		}
	}
	printf("Iterations = %ld Flag = %d\n",iterations,flag);
	printf("Key = %c%c%c%c\n",char(brute1),char(brute2),char(brute3),char(brute4));
	//printf("Brute values = %d %d %d %d\n",brute1,brute2,brute3,brute4);
	exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time = %fs\n",exec_time);
	
	return 0;
}
