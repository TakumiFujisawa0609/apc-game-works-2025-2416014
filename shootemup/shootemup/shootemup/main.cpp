#include<DxLib.h>
#include<cmath>
#include"Geometry.h"
#include <random>

///“–‚½‚è”»’èŠÖ”
///@param posA A‚ÌÀ•W
///@param radiusA A‚Ì”¼Œa
///@param posB B‚ÌÀ•W
///@param radiusB B‚Ì”¼Œa
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//“–‚½‚è”»’è‚ğÀ‘•‚µ‚Ä‚­‚¾‚³‚¢
	return (posB - posA).Length() < radiusA + radiusB;
}

float GetRadianFromDegree(float deg) {
	return deg* DX_PI_F / 180.f;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("2416014_¬–qŸ—˜");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//”wŒi—p
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH = LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24, playerH);

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	struct Bullet {
		Position2 pos;//À•W
		Vector2 vel;//‘¬“x
		Vector2 accel;//‰Á‘¬“x
		bool isActive = false;//¶‚«‚Ä‚é‚©`H
	};

	//’e‚Ì”¼Œa
	float bulletRadius = 5.0f;

	//©‹@‚Ì”¼Œa
	float playerRadius = 10.0f;

	//“K“–‚É256ŒÂ‚­‚ç‚¢ì‚Á‚Æ‚­
	Bullet bullets[256];

	Position2 enemypos(320,25);//“GÀ•W
	Position2 playerpos(320, 400);//©‹@À•W

	unsigned int frame = 0;//ƒtƒŒ[ƒ€ŠÇ——p

	char keystate[256];
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;
	constexpr float bullet_speed = 5.0;
	constexpr int bullet_interval = 12;// ’e‚Ì”­ËŠÔŠu
	constexpr int bullet_type_interval = 180;// ’e‚Ìí—Ş‚ÌØ‚è‘Ö‚¦ŠÔŠu
	SRand(GetTickCount());
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//”wŒi
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//ƒvƒŒƒCƒ„[
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//©‹@‚Ì–{‘Ì(“–‚½‚è”»’è)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		if (frame < bullet_type_interval) {
			//’e”­Ë
			if (frame % bullet_interval == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = Vector2(0, bullet_speed);// ^‰º‚É”­Ë
						b.isActive = true;
						break;// ‚±‚ê‚ª‚È‚¢‚Æˆê“x‚É‘S•”o‚Ä‚µ‚Ü‚¤B(d‚È‚é)
					}
				}
			}
		}
		else if (frame < bullet_type_interval * 2){
			//©‹@‘_‚¢’e
			//’e”­Ë
			if (frame % 12 == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = playerpos - enemypos;
						b.vel = b.vel.Normalized() * bullet_speed;
						b.isActive = true;
						break;
					}
				}
			}
		}
		else if (frame < bullet_type_interval * 3) {
			//•úËó’e
			//’e”­Ë
			constexpr float radial_div = 12.0; // ‰½•ûŒü‚ÉŒ‚‚Â‚©
			constexpr float diff_angle = DX_PI_F * 2.0f / radial_div;
			int cnt = 0;
			float angle = 0.0f;// ’e‚Ì•ûŒü
			if (frame % 12 == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = {cosf(angle),sinf(angle)};
						b.vel *= bullet_speed;
						b.isActive = true;
						cnt++;
						angle += diff_angle;
					}
					if (cnt > radial_div) {
						break;
					}
				}
			}
		}
		else if (frame < bullet_type_interval * 4) {
			//Nway’e
			//’e”­Ë
			constexpr int way_num = 5;//‰½way‚©
			constexpr float diff_angle = DX_PI_F * 2.0f / 36.0f;//10“x
			if (frame % 12 == 0) {
				int cnt = 0;
				auto vec = playerpos - enemypos;
				float angle = atan2f(vec.y,vec.x);
				angle -= diff_angle * (way_num / 2);
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = { cosf(angle),sinf(angle) };
						b.vel *= bullet_speed;
						b.isActive = true;
						cnt++;
						angle += diff_angle;
					}
					if (cnt >= way_num) {
						break;
					}
				}
			}
		}
		else if (frame < bullet_type_interval * 5) {
			//‚Î‚ç‚Ü‚«’e
			//’e”­Ë
			constexpr float radial_div = 12.0;
			constexpr float diff_angle = DX_PI_F * 2.0f / radial_div;

			if (frame % 12 == 0) {
				int cnt = 0;
				float angle = GetRadianFromDegree(GetRand(20) - 10);
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = { cosf(angle),sinf(angle) };
						b.vel *= (bullet_speed + GetRand(6)-3);
						b.isActive = true;
						cnt++;
						angle += diff_angle + GetRadianFromDegree(GetRand(20) - 10);
					}
					if (cnt > radial_div) {
						break;
					}
				}
			}
		}
		else if (frame < bullet_type_interval * 6) {
			//‰J’e
			//’e”­Ë
			enemypos.y = 125;
			constexpr float radial_div = 12.0; // ‰½•ûŒü‚ÉŒ‚‚Â‚©
			constexpr float diff_angle = DX_PI_F * 2.0f / radial_div;
			int cnt = 0;
			float angle = 0.0f;// ’e‚Ì•ûŒü
			if (frame % 12 == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.pos = enemypos;
						b.vel = { cosf(angle),sinf(angle) };
						b.vel *= bullet_speed;
						b.accel = { 0.0f,0.1f };
						b.isActive = true;
						cnt++;
						angle += diff_angle;
					}
					if (cnt > radial_div) {
						break;
					}
				}
			}
		}
		//else if (frame < bullet_type_interval * 7) {
		//	// •úËó’e‚Ì©‹@‘_‚¢’e
		//	// ’e”­Ë
		//	// ©‹@‚Ö‚ÌŠp“x
		//	Vector2 toPlayer = playerpos - enemypos;
		//	float baseAngle = atan2f(toPlayer.y, toPlayer.x);
		//	constexpr float radial_div = 12.0; // ‰½•ûŒü‚ÉŒ‚‚Â‚©
		//	constexpr float diff_angle = DX_PI_F * 2.0f / radial_div;
		//	// î‚Ì¶’[
		//	float angle = baseAngle - (diff_angle * (radial_div / 2));
		//	int cnt = 0;
		//	//float angle = 0.0f;// ’e‚Ì•ûŒü
		//	if (frame % 12 == 0) {
		//		for (auto& b : bullets) {
		//			if (!b.isActive) {
		//				b.pos = enemypos;
		//				b.vel = { cosf(angle),sinf(angle) };
		//				b.vel *= bullet_speed / 2;
		//				b.isActive = true;
		//				cnt++;
		//				angle += diff_angle;
		//			}
		//			if (cnt > radial_div) {
		//				break;
		//			}
		//		}
		//	}

		//}

		else{
			frame = 0;
		}

		//’e‚ÌXV‚¨‚æ‚Ñ•\¦
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			b.vel += b.accel;
			//’e‚ÌŒ»İÀ•W‚É’e‚ÌŒ»İ‘¬“x‚ğ‰ÁZ‚µ‚Ä‚­‚¾‚³‚¢
			b.pos += b.vel;

			float angle = 0.0f;
			//’e‚ÌŠp“x‚ğatan2‚ÅŒvZ‚µ‚Ä‚­‚¾‚³‚¢Bangle‚É’l‚ğ“ü‚ê‚é‚ñ‚¾‚æƒIƒD
			angle = atan2f(b.vel.y, b.vel.x);
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//’e‚Ì–{‘Ì(“–‚½‚è”»’è)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}

			//’e‚ğE‚·
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b = {};
			}

			//‚ ‚½‚èI
			//«‚ÌIsHit‚ÍÀ‘•‚ğ‘‚¢‚Ä‚Ü‚¹‚ñB©•ª‚Å‘‚¢‚Ä‚­‚¾‚³‚¢B
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//“–‚½‚Á‚½”½‰‚ğ‘‚¢‚Ä‚­‚¾‚³‚¢B
				DrawString(0,0,"“–‚½‚Á‚½",0xffffff);
				b = {};
			}
		}

		//“G‚Ì•\¦
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//“G‚Ì–{‘Ì(“–‚½‚è”»’è)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}