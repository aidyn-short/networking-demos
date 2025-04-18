1. Project Objectives
	The goal of my project is to show that I can create a networked game without a game engine and plugins and without the use of things services like Steam or Epic Games. While these are obviously important I have other projects that show I am capable of doing so and I want to show in the purest form my understanding of networking.

2. Tools and Technologies
	SDL2: For the rendering and gameplay of this game I am using SDL2. The SDL library has no built in networking and I will set up the networking myself for the client and the server.
	C++: For better performance and flexibility I will be using C++ in areas where I believe it is needed.

3. Scope
	The scope of this project is to create a simple top down 2D shooter where the players can interact with their environment and each other. They will be able to pick up weapons and health and these changes will be networked correctly.+
	

4. Deliverables
	At the end of this project there will be a playable demo and a short video showcase.

5.) Where I'm at
	Right now I have the server and client networking completed and the game is just sending the data for the player between the clients. I have basic gameplay systems set up like collision and camera scrolling and a good framework set up to easily add more objects and have them integrate into the networking seamlessly. 

6.) What's left
	The only features left now are weapons, health pick ups, and player death. I want to create 2 weapons so I can show how that interaction works but health pick ups and player death will be simple just adding to player's health and making them respawn in a random location when they die.