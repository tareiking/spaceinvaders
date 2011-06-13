#include "testApp.h"

#define numBunkers 5


unsigned int ENEMY_COUNT = 51;
int highScore = 0;
int enemySpacing = 50;
int enemyOffsetX = 100;
int enemyOffsetY = 50;
int direction = 1;
int enemyFiring = ofRandom(1, ENEMY_COUNT);
int lastSecondFired = 0;
bool keyIsDown[255];


void testApp::setup(){

	ofEnableSmoothing();
	ofSetFrameRate(30);
    ofEnableAlphaBlending();
	font.loadFont("font.ttf", 12);
    bigFont.loadFont("font.ttf", 20);
    
    ofHideCursor();
    ofBackground(235, 229,215);

    image1.loadImage("player.png");
    
    gameSetup();
	score = 0;
	numWingman = 0;
    tempLevel = 1;

}

void testApp::gameSetup(){
    setupEnemies();
    setupBunkers();
}


void testApp::update(){

    updateBullets();
    updateEnemies();
    checkEnemyBulletCollision();
    
    if(score > 100) numWingman = 1;
    if(score > 200) numWingman = 2;
    int enemyBullet = ofRandom(0, enemies.size());  
    
    if(ofGetElapsedTimeMillis() % 10 == 0) {
        if (lastSecondFired!=ofGetElapsedTimeMillis()){
			lastSecondFired = ofGetElapsedTimeMillis();
            Bullet randomBullet(enemies[enemyBullet].pos.x, enemies[enemyBullet].pos.y, false);
            bullets.push_back(randomBullet);
		}
    }
    
    // checks level details - move level 
    if(enemies.size()== 0) {
        enemies.clear();
        bullets.clear();
        ENEMY_COUNT += 17;
        tempLevel++;
        setupEnemies();
    }
    
    
    
    // removed key press delay and simultaneous space + direction corrected
    if(keyIsDown[356]) player.move("left");
    if(keyIsDown[358]) player.move("right");
    if(keyIsDown[32] && (keyIsDown[356] || keyIsDown[358]) || keyIsDown[32])
    {
		
        // wingman bullet shooting
		if (numWingman == 2) { 
			Bullet wingbullet(wingman.pos.x+35, ofGetHeight()-13, true);
			bullets.push_back(wingbullet);
			Bullet wingbullet1(wingman.pos.x-35, ofGetHeight()-13, true);
			bullets.push_back(wingbullet1);
            Bullet bullet(player.pos.x+2, ofGetHeight()-30, true);
            bullets.push_back(bullet);
		}
		
		if (numWingman == 1) {
			Bullet wingbullet(wingman.pos.x+35, ofGetHeight()-13, true);
            bullets.push_back(wingbullet);
            Bullet bullet(player.pos.x+2, ofGetHeight()-30, true);
            bullets.push_back(bullet);
		}
        
        if (numWingman == 0){
            Bullet bullet(player.pos.x+2, ofGetHeight()-30, true);
            bullets.push_back(bullet);
        }
    }    
    
    if(keyIsDown[114]) resetGame();
}
    
void testApp::draw(){
    
    if(player.health>0){
        drawPlayer();	
        drawEnemies();
        drawBullets();
        drawGui();
    }
    else {
        ofBackground(235, 229,215);
        
        if( score > highScore ) highScore = score;
        
        font.drawString("Game over!", ofGetWidth()/2,ofGetHeight()/2);
        font.drawString("Score:  " + ofToString(score), ofGetWidth()/2, ofGetHeight()/2+40);
        
        font.drawString("High Score:  " + ofToString(highScore), ofGetWidth()/2, ofGetHeight()/2+60);
        font.drawString("press \" r \" to reset game.", ofGetWidth()/2, ofGetHeight()/2+80);
        enemies.clear(); // stop getting score while dead
        bullets.clear(); // stop getting score while dead
    }
}

void testApp::drawGui(){
    
    ofSetColor(0, 0, 0, 200);
    font.drawString("Stich Invaders (Zak & Tarei)", 10 , 30);
	bigFont.drawString("Score:  " + ofToString(score), ofGetWidth()- 200, 30);
    bigFont.drawString("Level:  " + ofToString(tempLevel), ofGetWidth()- 400, 30);
    bigFont.drawString("Level:  " + ofToString(tempLevel), ofGetWidth()/2, ofGetHeight()/2);
    font.drawString("Lives:  ", ofGetWidth()-240, ofGetHeight()-15);   
    
    for (int i = 0; i < player.health; i++){
            ofSetColor(255,255,255);
            image1.draw(ofGetWidth()-(i*40), ofGetHeight()-30, 30, 20);
            ofSetColor(0,0,0, 200);
        }
    
   // ofBackground(235, 229,215);
}

void testApp::setupEnemies()
{
    int enemyPosY = 0;
	int enemyPosX = 0;    
    float enemyDirectionX = 1;
    int health = 4;
    
    for (int i = 0; i<ENEMY_COUNT; i++) {
		
		ofPoint pos;
		
		if (enemyPosX>(ofGetWidth()-2*enemyOffsetX)){
			enemyPosX = 0;
			enemyPosY += enemySpacing;
            if (health > 2) health -=1; 
		}
		
		pos.x = enemyPosX+enemyOffsetX;
		pos.y = enemyPosY+enemyOffsetY;
		enemyPosX+=enemySpacing;
		
        Enemy enemy(pos, health); //make new enemy
		enemies.push_back(enemy); //push it into vector
        
        
	}
	
}

void testApp::setupBunkers(){
    for (int i=0; i<=numBunkers; i++){

		Bunker bunker; // make new bunker		

		bunker.pos.x = i*200+100;
		bunker.pos.y = ofGetHeight()-100;
		bunker.width = 80;
		bunker.height = 50;
		bunker.health = 10;
		
		bunkers.push_back(bunker);
		
	}
}

void testApp::checkEnemyBulletCollision()
{
    int tolerance = 10;
    int offset = 20;
    // for each enemy in array     
    for (int i =  0; i < enemies.size(); i++) {        
        for (int j =  0; j < bullets.size(); j++) {
            for (int k = 0; k < bunkers.size(); k++){
                
                // enemy bullet hits bunkers
                if(ofDist(bunkers[k].pos.x, bunkers[k].pos.y, bullets[j].pos.x,bullets[j].pos.y) < 20){
                    // cout << "bunker hit!";
                    bullets.erase(bullets.begin()+j);
                    bunkers[k].health -= 1;
                    if(bunkers[k].health == 0) bunkers.erase(bunkers.begin()+k);
                    break;
                }
                
                // player hit
                else if ((ofDist(player.pos.x, player.pos.y, bullets[j].pos.x, bullets[j].pos.y) < 25) && (bullets[j].player == false))
                {
                    bullets.erase(bullets.begin()+j);
                    player.isHit = true;
                    player.health -= 1;
                    // cout << "\n" << "player hit";
                    break;
                }
                
                // player bullet hits enemy
                else if (ofDist(enemies[i].pos.x, enemies[i].pos.y, bullets[j].pos.x,bullets[j].pos.y) < 20) {
                    // only delete if the player bullet hits
                    if(bullets[j].player == true){

                        bullets.erase(bullets.begin()+j);
                        enemies[i].health -= 1;
                        if(enemies[i].health == 0)                         
                            enemies.erase(enemies.begin()+i);
                        score += 10;
                        // cout << "enemy killed" << "\n";
                    }
                    break;
                }
                // bullet falls off screen
                else if(bullets[j].pos.y < 0 || bullets[j].pos.y > ofGetHeight()-10){
                    bullets.erase(bullets.begin()+j);
                    break;
                }
            }
        }
    }
}


void testApp::updateBullets()
{
    for( int i = 0; i <  bullets.size(); i++){
        bullets[i].update();
    }
}


void testApp::updateEnemies()
{
    // check grid is offscreen
    bool shiftEnemiesDown = false;
    
	for (int i = 0; i<enemies.size(); i++) {
		if (( enemies[i].pos.x + 20)> ofGetWidth() || (enemies[i].pos.x - 20)<0){
			direction *= -1;
            shiftEnemiesDown = true;
            break;
		}
	}

    
	for (int i = 0; i<enemies.size(); i++) {
		enemies[i].pos.x += direction*enemies[i].velocity; // velocity == timefix 
		if (shiftEnemiesDown==true){
			enemies[i].pos.y += 10;
            enemies[i].velocity += 0.3;

		}	
	}
}


void testApp::drawBullets()
{
    if (bullets.size() > 0) {
		for (int i = 0; i < bullets.size(); i++){
			bullets[i].draw();
		}
	}
}


void testApp::drawEnemies()
{
    for (int i = 0; i < enemies.size(); i++)  enemies[i].draw();
    for (int i = 0; i < bunkers.size(); i++)  bunkers[i].draw();
}


void testApp::drawPlayer()
{
    player.draw();
	if(numWingman == 1) // draw right wingman
	{
		wingman.draw(player.pos, "right");
	}
	if(numWingman == 2) // draw left wingman
	{
		wingman.draw(player.pos, "left");
		wingman.draw(player.pos, "right");
	}
}

void testApp::resetGame(){
    enemies.clear();
    bunkers.clear();
    setupEnemies();
    setupBunkers();
    player.health=4;  
}

void testApp::keyPressed(int key){
	keyIsDown[key] = true;
    cout << key << "\n";
}

void testApp::keyReleased(int key){
    keyIsDown[key]  = false;
}

void testApp::mouseMoved(int x, int y ){
    player.moveMouse(x);
}

void testApp::mouseDragged(int x, int y, int button){
    
}

void testApp::mousePressed(int x, int y, int button){
    keyIsDown[32] = true;
}

void testApp::mouseReleased(int x, int y, int button){
    keyIsDown[32] = false;

}

void testApp::windowResized(int w, int h){

}

