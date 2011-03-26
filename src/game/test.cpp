/* Copyright (C) 2011  Thomas Glamsch (CruzR)
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

//this is used to test whether cmake worked
//you should see a black window when running it
//please consider adding other stuff to this source to make sure that everything works well

#include <SFML/Graphics.hpp>

int main (int argc, char * args[])
{
	//create a window
	sf::RenderWindow testwindow( sf::VideoMode( 640, 480, 32 ), "Xeonorgy Cmake Test" );

	while ( testwindow.IsOpened() )
	{
		//create a event handler
		sf::Event event;

		while ( testwindow.GetEvent( event ) )
		{	
			//if window is closed
			if ( event.Type == sf::Event::Closed )
			{
				testwindow.Close();
			}
		}

		//clear the screen
		testwindow.Clear();

		//display window contents
		testwindow.Display();
	}

	return EXIT_SUCCESS;
}
