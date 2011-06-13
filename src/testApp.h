#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"


// now git ready
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void checkEnemyBulletCollision();
        void updateBullets();
        void updateEnemies();
        void drawBullets();
        void drawEnemies();
        void drawPlayer();
        void setupEnemies();
        void setupBunkers();
        void drawGui();
        void resetGame();
        void gameSetup();
    
        void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
			
		int numWingman;	
		int score;
        int tempLevel;
    
		ofTrueTypeFont font, bigFont;
        ofImage image1;

    
    
// structures for game / enemy / bullet / player / wingman
// -------------------------------------------------------------
    
    struct Game {
        unsigned int level;
        
        unsigned int score;
        unsigned int difficulty; // 1 = easy, 2  = medium, 3 = hard
        unsigned int gameState; 
        // 0 = game start splash screen 
        // 1 = levelPlay 
        // 2 = shop mid level
        
        Game() {
            level = 1;


            gameState = 0;
            difficulty = 1;
            score = 0;
        }
        
    }gameEngine;
    
    
    
		struct Enemy {
			ofPoint pos;
            int direction;
            float velocity;
            ofImage img;
            float health;
            float baseHealth;
            
			Enemy(ofPoint _enemyPos, int enemyHealth){
				pos = _enemyPos;
                direction = 1;
                if( enemyHealth > 3) img.loadImage("enemy4.png");
                if( enemyHealth == 3 ) img.loadImage("enemy3.png");
                if( enemyHealth < 3 ) img.loadImage("enemy2.png");                

                    
                    
                    // img.setImageType(OF_IMAGE_COLOR_ALPHA);
                velocity = 0.5;
                health = enemyHealth;   
                baseHealth = health;
            }
			
			void draw(){
                ofSetColor(255, 255, 255, ofMap(health/baseHealth, 0, 1, 50, 255));
                ofSetRectMode(OF_RECTMODE_CENTER);
                img.draw(pos.x, pos.y, 40, 50);
                ofSetRectMode(OF_RECTMODE_CORNER);

//                ofRect(pos.x, pos.y, 30, 30);
                // health bars
                ofSetColor(0,255,0, 125);
                ofRect(pos.x-15, pos.y + 10, ofMap(health/baseHealth, 0, 1, 5, 30), 5);

            }
            
            void update(){
                if(pos.x > ofGetWidth() || pos.x < 0)
                {
                    direction *= -1;
                    pos.y += 10;
                    velocity+=0.9;
                }
                pos.x += (direction*velocity);
            }
		};
		vector<Enemy> enemies;	
	
        struct Bunker {
            ofPoint pos;
            int health;
            int width;
            int height;
            ofImage img;
            
            Bunker(){
                img.loadImage("bunker.png");
            }
            void draw(){
                ofSetColor(255,255,255, ofMap(health, 0, 10, 0, 255));
                img.draw(pos.x-(width/2), pos.y-(height), width, height);
//                ofRect(pos.x-(width/2), pos.y-(height/2), width, height);
            }
        };
        vector <Bunker> bunkers;

        
		struct Bullet{
			ofPoint pos;
            int velocity;
			int size;
            int direction;
            bool player;
            
            Bullet(float _x, float _y = ofGetHeight(), bool _player = true){
                player = _player;
                pos.y = _y;
				pos.x = _x-3 ;
                velocity = 15;
                if(player == true) direction = 1; else if(player == false) direction = -1;
                
            }
		
			void update(){

                switch (direction) {
                    case 1:
                        if(pos.y > 0)
                        {
                            pos.y -= velocity * direction;
                        }
                        break;
                        
                    default:
                        if(pos.y < ofGetHeight()-5)
                        {
                         pos.y -= velocity * direction;   
                        }
                        break;
                }
			}	
			
			void draw(){
                ofSetColor(0, 0, 0, ofMap(pos.y, 0, ofGetHeight(), 40, 255));
                ofRect(pos.x, pos.y-20, 3, 8);

			}			
		};
		vector<Bullet> bullets; 
	
		struct Player {
			ofPoint pos;
			int health;
            ofImage img;
            bool isHit;
			
			Player(){
				pos.x = ofGetWidth()/2;
				pos.y = ofGetHeight()-20;
                img.loadImage("player.png");
                health = 5;
                isHit = false;
			}
			
			void move(string dir){
				if(dir == "left" && (pos.x > 0)){ pos.x -= 20;}
				if(dir == "right" && (pos.x < ofGetWidth())){ pos.x += 20;}
			}
            
            void moveMouse (int _x){
                pos.x = _x;
            }
				   
			void draw(){
                ofSetColor(255, 255, 255);
                //if (isHit == true) ofSetColor(255, 0, 0);
                // ofSetColor(255, 255, 255, 125);
                img.draw(pos.x-27, pos.y-25, 54, 43); //43, 54);
             //   ofTriangle(pos.x-10, pos.y, pos.x, pos.y-20, pos.x+10, pos.y);
                //isHit = false;
                
			}
			
		}player;
	
		struct Wingman{		
			
			ofPoint pos;
			int offsetX;
            ofImage img;

			
			Wingman(){
                img.loadImage("player.png");				
			}
			
			void draw(ofPoint playerPos, string location){
				pos.x = playerPos.x;
				pos.y = playerPos.y;
				
 				if(location == "right"){
					offsetX = 40;
				} else if ( location == "left" ){
					offsetX = -40;
				}
				ofSetRectMode(OF_RECTMODE_CENTER);
                img.draw(pos.x + offsetX, pos.y+5, 20, 14);
//                ofRect(pos.x + offsetX, pos.y, 10, 10);
					
			}
			
			void fire(string location){
			}
			
		} wingman;
};

#endif
