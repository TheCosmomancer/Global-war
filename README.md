# Global war
This is a project for my Basic programming class made with [raylib](https://www.raylib.com/).
This is my first project and so the controls are not very intuitive please visit the tutorial if you haven't played before and/or do not know the controls.
## Main menu
Inside you can start the game , toggle fast combat (also toggleable in-game) , toggle music (not toggleable in-game) , visit the scoreboard , visit the tutorial or exit the game ( note that you can exit the game at any time by pressing [ESC] or clicking on the X icon of the window ).
## Core game cycle
The game starts by distributing countries between the three players ( red , green and blue ) and placing troops on them ; Then the player take turns going through their phases. Player turn order is randomized but each turn consists of the following phases :
#### 1. Placing troops
In this phase the current player are given troops ( with bonuses for controlling many countries or controlling all countries in a certain continent ) and can place them anyway they like among the countries they control
#### 2.Combat phase
Here the current player can attack neighboring countries of their countries that they don't control or activate fast combat ( only advised when both sides have a large number of troops ). Maximum number of attackers is three and defendant is Two , however one at least one trooper must remain in the attacker's country. Odds generally favor the attacker with a great emphasis on attacking and defending with as many troops as possible so maximum number or attackers and defenders are automatically chosen.
#### 3.Marching phase
Lastly the current player may move any number of troops from and to a country they control subject to the following :
a. There must remain at least one trooper on each country at any given time.
b. The march should be possible without marching through enemy territory.
#### 4.End phase
After passing the marching phase , the current player draws a card if they captured a country this turn. If the cease fire card is drawn the game ends on the spot ; otherwise they get some starts which can be exchanged on the first phase to get more troops with more stars giving more troops per star. the minimum number of stars that can be exchange is 2 and the maximum is 10.
### Ending
The game ends when a player controls 25 or more countries or cease fire is drawn. Winner is announced , the scoreboard is updated and players are prompted to go back to the main menu.
## License

Published under the [MIT](https://choosealicense.com/licenses/mit/) License.
## Music
The music used in the game is [The sun falls](https://pixabay.com/music/build-up-scenes-the-sun-falls-149603/) by "[LorenzooCs](https://pixabay.com/users/lorenzoocs-36307647/)" via [Pixabay](https://pixabay.com/).
