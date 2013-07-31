#include <sstream>
#include "LightManager.h"

int main()
{
    // Cr�ation de la fen�tre de rendu.
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "Holyspirit Light Test");

    // Cr�ation de l'horloge pour g�rer le temps.
    sf::Clock Clock;

    // Cr�ation d'un tableau qui contient les images des murs.
    sf::Vertex Wall[] = {
        sf::Vertex(sf::Vector2f(450, 325)), sf::Vertex(sf::Vector2f(300, 200)),
        sf::Vertex(sf::Vector2f(250, 240)), sf::Vertex(sf::Vector2f(400, 365)),
        sf::Vertex(sf::Vector2f(125, 125)), sf::Vertex(sf::Vector2f(100, 100)),
        sf::Vertex(sf::Vector2f(500, 200)), sf::Vertex(sf::Vector2f(600, 300)),
        sf::Vertex(sf::Vector2f(500, 300)), sf::Vertex(sf::Vector2f(600, 200)),
        sf::Vertex(sf::Vector2f(125, 425)), sf::Vertex(sf::Vector2f(125, 475)),
        sf::Vertex(sf::Vector2f(125, 475)), sf::Vertex(sf::Vector2f(175, 475))
    };

    // Cr�ation du Light_Manager, c'est lui qui va s'occupper de g�rer les lumi�res.
    Light_Manager *Manager;
    Manager=Light_Manager::GetInstance();

    Manager->m_lightSmooth=2;
    Manager->m_basicLight=sf::Color(32,32,32);

    // On ajoute des murs au Light_Manager, ce sont ces murs qui sont pris en compte lors du calcul des lumi�res
    // (Position du point 1, Position du point 2).

    Manager->Add_Wall(sf::Vector2f(450,325),sf::Vector2f(300,200));
    Manager->Add_Wall(sf::Vector2f(250,240),sf::Vector2f(400,365));
    Manager->Add_Wall(sf::Vector2f(100,100),sf::Vector2f(125,125));
    Manager->Add_Wall(sf::Vector2f(500,200),sf::Vector2f(600,300));
    Manager->Add_Wall(sf::Vector2f(500,300),sf::Vector2f(600,200));
    Manager->Add_Wall(sf::Vector2f(125,425),sf::Vector2f(125,475));
    Manager->Add_Wall(sf::Vector2f(125,475),sf::Vector2f(175,475));


    // Cr�ation des Light_Entity, ce sont elles qui permettent de modifier les lumi�res par apr�s, comme changer de position, de couleur, etc
    Light_Entity light,light2,light3,directional_light;

    // On ajoute une lumi�re dynamique au Light_Manager et on dit que c'est "light" qui la repr�sente.
    light=Manager->Add_Dynamic_Light();
    light2=Manager->Add_Dynamic_Light();
    // On ajoute une lumi�re dynamique � la position (600,600), d'intensit� 255, de rayon 160, de qualit� 16 et de couleur verte.
    light3=Manager->Add_Dynamic_Light(sf::Vector2f(600,200),255,160,16,sf::Color(0,255,0));

    // On ajoute une lumi�re statique � la position (110,490), d'intensit� 160, de rayon 96, de qualit� 8 et de couleur blanche. On ne la raccorde pas � une
    // Light_Entity car c'est une lumi�re statique et qui n'est donc pas modifiable.
    Manager->Add_Static_Light(sf::Vector2f(110,490),160,96,8,sf::Color(255,255,255));

    // On donne la position (375,275) � la lumi�re raccord�e � "light".
    Manager->SetPosition(light,sf::Vector2f(375,275));

    // On donne une intensit� de 255 � la lumi�re raccord�e � "light".
    Manager->SetIntensity(light,255);

    // On donne un rayon de 128 � la lumi�re raccord�e � "light".
    Manager->SetRadius(light,128);

    // On donne une qualit� de 16 � la lumi�re raccord�e � "light".
    Manager->SetQuality(light,16);

    // On donne une couleur rouge � la lumi�re raccord�e � "light".
    Manager->SetColor(light,sf::Color(255,0,0));


    // M�me chose que juste au dessus, mais avec "light2"
    Manager->SetPosition(light2,sf::Vector2f(175,50));
    Manager->SetIntensity(light2,255);
    Manager->SetRadius(light2,128);
    Manager->SetQuality(light2,16);
    Manager->SetColor(light2,sf::Color(0,0,255));


    directional_light = Manager->Add_Dynamic_Directional_Light(sf::Vector2f(750,310),255,384,90,45,sf::Color(255,128,255));



    // Cr�ation d'une image qui servira d'image de fond, on charge donc l'image "test.png".
    sf::Texture Image;
    Image.loadFromFile("test.png");
    sf::Sprite background;
    background.setTexture(Image);
    background.setScale(/*800/Image.GetWidth(),
			  600/Image.getHeight()*/ 1,1);


    // Cr�ation de variables pour la gestion du temps.
    float LightRefresh=0;

    //Cr�ation de bool pour faire rebondir les lumi�res
    bool allerX=true,allerY=true;
    bool allerX2=true,allerY2=true;

    float angle = 90;

    // Ex�cution de la boucle principale
    while (App.isOpen())
    {
        // Traitement des �v�nements
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Fen�tre ferm�e : on quitte
            if (Event.type == sf::Event::Closed)
                App.close();
        }

        // On ajoute le temps �coul�
        LightRefresh+=Clock.getElapsedTime().asSeconds();

        angle += Clock.getElapsedTime().asSeconds() * 20;


        //On d�place la "light" en fonction du temps, avec allerX et allerY pour savoir si on monte ou on descend.
        if(allerX)
            Manager->SetPosition(light,sf::Vector2f(Manager->GetPosition(light).x+100*Clock.getElapsedTime().asSeconds(),
                                                    Manager->GetPosition(light).y));
        else
            Manager->SetPosition(light,sf::Vector2f(Manager->GetPosition(light).x-100*Clock.getElapsedTime().asSeconds(),
                                                    Manager->GetPosition(light).y));

        if(allerY)
            Manager->SetPosition(light,sf::Vector2f(Manager->GetPosition(light).x,
                                                    Manager->GetPosition(light).y+100*Clock.getElapsedTime().asSeconds()));
        else
            Manager->SetPosition(light,sf::Vector2f(Manager->GetPosition(light).x,
                                                    Manager->GetPosition(light).y-100*Clock.getElapsedTime().asSeconds()));

        //Si la "light" sort d'une certaine zone, on modifie allerX et/ou allerY pour faire rebondir.
        if(Manager->GetPosition(light).x>800)
            allerX=false;
        if(Manager->GetPosition(light).x<0)
            allerX=true;

        if(Manager->GetPosition(light).y>600)
            allerY=false;
        if(Manager->GetPosition(light).y<0)
            allerY=true;

        // Ce ne sert que pour la d�mo technique et n'intervient pas dans l'utilisation du moteur de lumi�res


        // Idem mais avec light2
        if(allerX2)
            Manager->SetPosition(light2,sf::Vector2f(Manager->GetPosition(light2).x+125*Clock.getElapsedTime().asSeconds(),
                                                     Manager->GetPosition(light2).y));
        else
            Manager->SetPosition(light2,sf::Vector2f(Manager->GetPosition(light2).x-125*Clock.getElapsedTime().asSeconds(),
                                                     Manager->GetPosition(light2).y));

        if(allerY2)
            Manager->SetPosition(light2,sf::Vector2f(Manager->GetPosition(light2).x,
                                                     Manager->GetPosition(light2).y+125*Clock.getElapsedTime().asSeconds()));
        else
            Manager->SetPosition(light2,sf::Vector2f(Manager->GetPosition(light2).x,
                                                     Manager->GetPosition(light2).y-125*Clock.getElapsedTime().asSeconds()));

        if(Manager->GetPosition(light2).x>800)
            allerX2=false;
        if(Manager->GetPosition(light2).x<0)
            allerX2=true;

        if(Manager->GetPosition(light2).y>600)
            allerY2=false;
        if(Manager->GetPosition(light2).y<0)
            allerY2=true;


        // On place light 3 � cette position, avec un tout petit d�calage, pour �viter d'avoir une lumi�re pile sur l'axe d'un mur, �a peut engendrer des probl�mes pour le moment.
        Manager->SetPosition(light3,sf::Vector2f(sf::Mouse::getPosition(App).x,
                                                 sf::Mouse::getPosition(App).y));

        Manager->SetOtherParameter(directional_light,ANGLE,angle);

        Clock.restart();

        //Vous pouvez changer le 0.025 par une autre valeur si vous voulez que les lumi�res ne se rafraichissent que toutes les x secondes, au lieu de le faire � chaque tour de boucle.
        if(LightRefresh>0.025)
        {
            // On re-calcule les lumi�res
            Manager->Generate();
            LightRefresh=0;
        }

        // Efface l'�cran (remplissage avec du noir)
        App.clear();

        // On affiche le fond
        App.draw(background);

        // On affiches les murs
        App.draw(Wall, 10,sf::Lines);

        // On affiche les lumi�res
        Manager->Draw(&App);


        // Affichage du contenu de la fen�tre � l'�cran
        App.display();
    }

     Manager->Kill();

    return EXIT_SUCCESS;
}



