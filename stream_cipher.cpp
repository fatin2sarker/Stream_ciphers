/*
 *project_encrypt.cpp
 *Created on: Nov. 4, 2019
 *Author: Fatin Sarker
 *Copyright © 2019 Fatin Sarker. All rights reserved
 *1A, Computer Engineering
 *University of Waterloo
 */
 
 
#include <iostream>
#include <cmath>
#include <cctype>


char *encode(char *plaintext, unsigned long key);               //plaintext > padded > ASCII armour > ciphertext
char *decode(char *ciphertext, unsigned long key);             //ciphertext > ASCII armour > padded > ciphertext
int get_length (char *string);

#ifndef MARMOSET_TESTING     //code tested with multiple pre-structed marmoset testing 
int main()
{
		char str0[]{ "Hello world!" };
		char str1[]{"l=]V]&9!>trOu*9"};

		std::cout << "\"" <<"Plaintext: "<< str0 << "\"" << std::endl;
    
    // Key: 51323
    
		char *ciphertext{ encode( str0, 51323 ) };
		std::cout << "\"" <<"Encoded: " << ciphertext << "\"" << std::endl;

		std::cout << "\"" << "Ciphertext: "<< str1 << "\"" << std::endl;

		char *plaintext{ decode( str1, 51323 ) };
		std::cout << "\""<<"Decoded: " << plaintext << "\"" << std::endl;

		delete[] plaintext;
		plaintext = nullptr;
		delete[] ciphertext;
		ciphertext = nullptr;

		return 0;
}
#endif

/////////////////////////////////////////////

//Get the length of the string input 

int get_length (char *string)
{
	int count = 0;
	while (string[count] != '\0')
	{
		count++;
	}
	return count;
}

////////////////////////////////////
char *encode(char *plaintext, unsigned long key)
{
	unsigned char S[256];
	int i = 0;
	int j = 0;
		for (int k = 0; k < 256; k++)
		{
			S[k] = k;      //unscrambled array 0 to 255//Identity permutation
		}
    
	//scramble the letters for encryption
  
		for (int x = 0; x < 256; x++)
		{
			int k = i % 64;
			int bit = (key >> k) & 1;  //kth bit of key (decimal)
			j = (j + S[i]+ bit) % 256;

			int temp;
			temp = S[i];
			S[i] = S[j];
			S[j] = temp;   //Swap(S[i],S[j]);
			i = (i + 1) % 256;
		}
    
    
	/*for (int h = 0; h <256; h++)
	{
		std::cout<<static_cast<unsigned long>(S[h])<<std::endl;        //manually check (cout) if the letters are scamrbled accordingly
	}*/

		int length = get_length(plaintext);  //length of plaintext
		int new_length;
			if ((length%4) != 0) //length of padded array
				new_length = length + (4 - (length % 4));
			else
				new_length = length;
		unsigned char *padded = new unsigned char[new_length];

			for (int c = 0; c <= length; c++)
			{
				padded[c] = plaintext[c];
			}
			for (int d = length; d <= new_length; d++)
			{
				padded[d] = '\0';
			}
      
	//encryption//PRGA  */
  
	int counter =0;
		while (counter != new_length)
		{
			i = (i + 1) % 256;
			j = (j + S[i]) % 256;
			int temp;
			temp = S[i];
			S[i] = S[j];
			S[j] = temp;   //Swap(S[i],S[j]);
			int r = (S[i] + S[j]) % 256;
			unsigned char R = S[r];
			padded[counter] = padded[counter]^R;  //^ every byte of padded array
			counter++;
		}
    
		//std::cout<<padded<<std::endl;   //check

     //ASCII armour the encrypted padded array
     
	int size_ciphertext = 5.0*std::ceil(new_length/4.0)+1;
	char *ciphertext = new char[size_ciphertext];
	unsigned int sum =0;
		for (int b =0; b < new_length/4; b++)
		{
			sum = (padded[b*4]<<24) + (padded[b*4 + 1]<<16) + (padded[b*4 + 2]<<8) + (padded[b*4 + 3]);
			//std::cout<<"sum: "<<sum<<std::endl;
			int rem;
				for (int f = 4; f>=0; f--)
				{
					rem = (sum % 85) + 33;
					sum = sum/85;
					//std::cout<<rem<<std::endl;
					ciphertext [(b*5) + f] = rem;
				}
		}
	ciphertext[size_ciphertext] = '\0';
	return ciphertext;
}

/////////////////////////////////////////////////
char *decode(char *ciphertext, unsigned long key)
{
	//decode ASCII armour
	int length = get_length(ciphertext);
	int new_length = (length/5)*(4)+1;
	char *plaintext = new char[new_length];
	//unsigned char *xor_array = new unsigned char[new_length];
	int z=0;
		for (int f = 0; f <length; f+=5)
		{
			unsigned int sum = 0;
			for (int g = 4; g >= 0; g--)
			{
				sum = sum + (ciphertext[f+g] - 33) * pow(85,4-g);
			}

			int m = 24;
			for (int h = 0; h < 4; h++)
			{
				plaintext[h+z] = (sum >> m);
				m = m - 8;
			}
			//std::cout<<static_cast<unsigned int>(sum)<<std::endl;
			z = z+ 4;
		}

	unsigned char S[256];
	int i = 0;
	int j = 0;
		for (int k = 0; k < 256; k++)
		{
			S[k] = k;  //unscrambled array 0 to 255//Identity permutation
		}
		//scramble
		for (int x = 0; x < 256; x++)
		{
			int k = i % 64;
			int bit = (key >> k) & 1;  //kth bit of key (decimal)
			j = (j + S[i]+ bit) % 256;

			int temp;
			temp = S[i];
			S[i] = S[j];
			S[j] = temp;   //Swap(S[i],S[j]);
			i = (i + 1) % 256;
		}

	int counter =0;
			while (counter != new_length)
			{
				i = (i + 1) % 256;
				j = (j + S[i]) % 256;
				int temp;
				temp = S[i];
				S[i] = S[j];
				S[j] = temp;   //Swap(S[i],S[j]);
				int r = (S[i] + S[j]) % 256;
				unsigned char R = S[r];
				plaintext[counter] = plaintext[counter]^R;
				counter++;
			}
	plaintext[new_length-1] = '\0';
	return plaintext;
}


/*
THIS PART REFERS TO GENERAL OUTPUT without the marmoset testing. 
{
	char str0[]{ "Hello world!" };
	char str1[]{"l=]V]&9!>trOu*9"};

	std::cout << "\"" <<"Plaintext: "<< str0 << "\"" << std::endl;

	char *ciphertext{ encode( str0, 51323 ) };
	std::cout << "\"" <<"Encoded: " << ciphertext << "\"" << std::endl;

	std::cout << "\"" << "Ciphertext: "<< str1 << "\"" << std::endl;

	char *plaintext{ decode( str1, 51323 ) };
	std::cout << "\""<<"Decoded: " << plaintext << "\"" << std::endl;

	delete[] plaintext;
	plaintext = nullptr;
	delete[] ciphertext;
	ciphertext = nullptr;
}*/




