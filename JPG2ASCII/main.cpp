/*********************************************************************

This file is part of JPG2ASCII (GNU GPL) - https://www.youtube.com/user/Muzkaw.

JPG2ASCII is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

JPG2ASCII is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JPG2ASCII. If not, see <http://www.gnu.org/licenses/>.

JPG2ASCII codded by Muzkaw : https://www.youtube.com/user/Muzkaw
You need to link SFML 2.0 or above to run the program

********************************************************************/

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace std ;
using namespace sf ;

#define RED_COEFF 0.2125 //red, green and blue don't have the same weight when dealing with color -> gray scale conversion. These are standard weights (read http://en.wikipedia.org/wiki/Grayscale#Grayscale_as_single_channels_of_multichannel_color_images)
#define GREEN_COEFF 0.7154
#define BLUE_COEFF 0.0721

char getCorrespondingAsciiChar(int grayScale, string among) // a fonction that link a char to a range of gray scale. "among" is a string saving the ASCII characters that the user wiches to use.
{
	return among[(int)(grayScale/255.f*among.size())] ;
}

int main()
{
	int Xresolution ; // getting all the infos from the user
	string path ;
	string outputPath ;
	string customChar ;
	string among = "@#£=+|:. " ;
	cout<<"Write the path of the picture you want to convert : " ;
	cin>>path ;
	cout<<"Write the path of the output ASCII text file : " ;
	cin>>outputPath ;
	cout<<"Choose the size (in px) that an ASCII character will represent : ";
	cin>>Xresolution;
	cout<<"Do you want to use your own set of characters ? (yes/no)";
	cin>>customChar ;
	if(customChar == "yes" || customChar == "y" || customChar == "YES" || customChar == "Y" || customChar == "Yes")
	{
		cout<<"Write you characters from the darker to the brighter (i.e. : @%#Oo*+=-:. ) : " ;
		cin>>among;
	}
	Image image ; 
	image.loadFromFile(path) ;
	
	Image bwImage(image) ;

	cout<<"Conversion to black and white..."<<endl;

	for(int i(0) ; i < image.getSize().x ; i++) // converts each pixel to grayscale with weights
	{
		for(int j(0) ; j < image.getSize().y ; j++)
		{
			bwImage.setPixel(i,j,Color::Color(  image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF ,
												image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF ,
												image.getPixel(i,j).r*RED_COEFF + image.getPixel(i,j).g*GREEN_COEFF + image.getPixel(i,j).b*BLUE_COEFF) ) ;
		}
	}
	cout<<"Done !"<<endl;
	cout<<"Conversion to low resolution..."<<endl;
	int Yresolution = 2*Xresolution ; // letters spacing in notePad works best with these settings
	Image bwLowResImage(image) ;
	int grey = 0 ;
	for(int i(0) ; i < image.getSize().x-Xresolution ; i+=Xresolution) // converting to a lower resolution if the users want less char in his final .txt : pixels are averaged in squares of Xresolution*Yresolution
	{
		for(int j(0) ; j < image.getSize().y-Yresolution ; j+=Yresolution)
		{
			for(int x(0) ; x < Xresolution ; x++)
			{
				for(int y(0) ; y < Yresolution ; y++)
				{
					grey += bwImage.getPixel(i+x,j+y).r ;
				}
			}
			grey /= Xresolution * Yresolution + 1;
			for(int x(0) ; x < Xresolution ; x++)
			{
				for(int y(0) ; y < Yresolution ; y++)
				{
					bwLowResImage.setPixel(i+x,j+y,Color( grey ,grey , grey ) ) ;
				}
			}
		}
	}
	cout<<"Done !"<<endl;
	cout<<"Conversion to ASCII..."<<endl;
	string asciiArt ;
	
	for(int j(0) ; j < bwLowResImage.getSize().y ; j+=Yresolution)
	{
		for(int i(0) ; i < bwLowResImage.getSize().x ; i+=Xresolution)
		{
			asciiArt += getCorrespondingAsciiChar(bwLowResImage.getPixel(i,j).r,among) ;
		}
		asciiArt+="\n" ;
	}
	ofstream output(outputPath) ;
	if(output) output << asciiArt ;
	cout<<"Finished ! You will find the result at : "<<outputPath<<endl;
	system("PAUSE") ;
	return 0 ;
}