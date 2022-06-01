# CodeWing
CodeWing is a collaborative editor  that allows several people to edit, view, compile a programming code simultaneously  over a computer network. Users can edit the same code, run the code, can chat through the text box in real time. 

This is our System Practicum (CS307) Group Project.

## APP Sources
1. App is Deploy on GitHub - [CodeWing ;)](https://prashant280920.github.io/CodeWing/)
2. Source Code - [prashant280920/CodeWing](https://github.com/prashant280920/CodeWing)
3. PPT - [Link]()
4. Presentation + Demo on [Youtube](https://youtu.be/GrOmdXlWy04) 

## Our Team 

1. [Prashant Kumar](https://github.com/prashant280920)
2. [Saloni Patidar](https://github.com/Saloni1771)
3. [Gaurav Sahitya](https://github.com/gaurav-sahitya)
4. [Sagar Tarafder](https://github.com/sagartarafder6)
5. [Ravi Kumar](https://github.com/RaviKumar7870)
6. [Shubham Saurav](https://github.com/sauravshub1)

## Features
1. Video Call Application: Multiple people can interact in a room to talk with each other.
2. RealTime WhiteBoard: Diffrent people can interact to explain their thoughts on a whiteboard. 
3. RealTime Code Editor: Editor where different people write code in a room.
4. Code Compilation and Download: Allow compilation of code, also download it.
5. Chat Application: Allow differnt user to chat with each other in a room.

## Implimentaion
### Frontent
1. We have used [SvelteJS](https://svelte.dev/) for frontend.
2. To implement the Collaborating Text Editor we have used CodeMirror of [Firepad](https://firepad.io/examples/#-rrTN85TPay).
3. Chat, Video, and White board uses WebSocjet to connect with server.

### Backend
1. We have used four different server in [NodeJS](https://nodejs.org/en/) each of them hosted on [Heroku](https://www.heroku.com/) 
2. [White Board Server](https://github.com/salon280920/wb_server) for implementing white board.
3. [Video Server](https://github.com/salon280920/video_server) for implementing Video application.
4. [Chat Server](https://github.com/salon280920/chat_server) for implementing chat application.
5. [Peer Js](https://peerjs.com/#:~:text=PeerJS%20simplifies%20WebRTC%20peer%2Dto,connection%20to%20a%20remote%20peer.) PeerJS simplifies WebRTC peer-to-peer data, video, and audio calls

