/*---------------------------------------------------------*/
/* ----------------  Proyecto final            -----------*/
/*--------------Alumno: Domínguez Reyes Cynthia Berenice 
----------------No. Cuenta: 314148632 ---------------*/
/*--------------Alumno: Peralta Espinosa Carlos Alejandro 
----------------No. Cuenta: 307276825 ---------------*/


#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

/* //Audio
#ifdef WIN32
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#endif

#include <iostream>
#include <irrKlang.h>

using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

//audio
float posOnCircle = 0;
const float radius = 5;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Calcula el tiempo actual(puedes usar glfwGetTime() u otra fuente de tiempo)
float currentTime = glfwGetTime();
float spd = 0.2f; // Velocidad de desplazamiento del sol
// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
//glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f);
// definicion de la luz solar
glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f);
glm::vec3 light(0.0f, 0.0f, 0.0f);
glm::vec3 l(0.0f, 0.0f, 0.0f);

static float cont = 0.0;
static bool on = false;
static bool o = false;

// Ajusta la velocidad de cambio de color
float speed = 0.001f;


bool	anim = false;

int circuito_auto = 0;
int recorrido = 0;
////////////
float myVar = 0.0f;
float sol = 0.0f;

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// Variabiles para animacion de avion
bool animacionAvion = true;
float avionMovX = 345.0f;
float avionMovY = 0.1f;
float avionMovZ = -150.0f;
float orientacionAvionX = 0.0f;
float orientacionAvionY = 180.0f;
float movAuxX = 0.0f;
float movAuxY = 0.0f;
float movAuxZ = 0.0f;
float anguloAvion = 0.0f;
float giroAvion = 0.0f;
int estadoAvion = 0;

// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

float	posOpelX = 0.0f,
posOpelY = 0.0f,
posOpelZ = 0.0f,
rotOpelRueda = 0.0f,
rotOpelY = 0.0f,
rotOpelFaro = 0.0f,
rotOpelPuerta = 0.0f;
int		incPosOpelX = 0,
incPosOpelY = 0,
incPosOpelZ = 0,
incRotOpelRueda = 0,
incRotOpelY = 0,
incRotOpelFaro = 0,
incRotOpelPuerta = 0;

// posiciones
float	movP_x = 0.0f,
movP_z = 0.0f,
giro = 180.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

	float posOpelX = 0.0f;
	float posOpelY = 0.0f;
	float posOpelZ = 0.0f;
	float rotOpelRueda = 0.0f;
	float rotOpelY = 0.0f;
	float rotOpelFaro = 0.0f;
	float rotOpelPuerta = 0.0f;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	posOpelX = KeyFrame[0].posOpelX;
	posOpelY = KeyFrame[0].posOpelY;
	posOpelZ = KeyFrame[0].posOpelZ;
	rotOpelRueda = KeyFrame[0].rotOpelRueda;
	rotOpelY = KeyFrame[0].rotOpelY;
	rotOpelFaro = KeyFrame[0].rotOpelFaro;
	rotOpelPuerta = KeyFrame[0].rotOpelPuerta;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	incPosOpelX = (KeyFrame[playIndex + 1].posOpelX - KeyFrame[playIndex].posOpelX) / i_max_steps;
	incPosOpelY = (KeyFrame[playIndex + 1].posOpelY - KeyFrame[playIndex].posOpelY) / i_max_steps;
	incPosOpelZ = (KeyFrame[playIndex + 1].posOpelZ - KeyFrame[playIndex].posOpelZ) / i_max_steps;
	incRotOpelRueda = (KeyFrame[playIndex + 1].rotOpelRueda - KeyFrame[playIndex].rotOpelRueda) / i_max_steps;
	incRotOpelY = (KeyFrame[playIndex + 1].rotOpelY - KeyFrame[playIndex].rotOpelY) / i_max_steps;
	incRotOpelFaro = (KeyFrame[playIndex + 1].rotOpelFaro - KeyFrame[playIndex].rotOpelFaro) / i_max_steps;
	incRotOpelPuerta = (KeyFrame[playIndex + 1].rotOpelPuerta - KeyFrame[playIndex].rotOpelPuerta) / i_max_steps;
}

void animate(void)
{
	lightPosition.x = 300.0f * sin(myVar);
	lightPosition.z = 100.0f * cos(myVar);


	myVar += 0.05f;

	lightDirection.x = 300.0f * sin(sol); // Ajusta para moverse horizontalmente
	lightDirection.y = 100.0f * cos(sol); // Ajusta para moverse verticalmente

	sol += 0.005f;



	if (on)
	{
		cont += speed;

		if (cont <= 5.0f)
		{
			light = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else if (cont <= 10.0f)
		{
			light = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else if (cont <= 15.0f)
		{
			light = glm::vec3(1.0f, 0.0f, 1.0f);
		}
		else if (cont <= 20.0f)
		{
			light = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else if (cont <= 25.0f)
		{
			light = glm::vec3(0.0f, 1.0f, 1.0f);
		}

		if (cont >= 25.0f)
		{
			cont = 0.0;
		}

		

	}

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			posOpelX += incPosOpelX;
			posOpelY += incPosOpelX;
			posOpelZ += incPosOpelZ;
			rotOpelRueda += incRotOpelRueda;
			rotOpelY += incRotOpelY;
			rotOpelFaro += incRotOpelFaro;
			rotOpelPuerta += incRotOpelPuerta;
			i_curr_steps++;
		}
	}

	if (animacionAvion)
	{
		if (estadoAvion == 0) {
			avionMovZ -= 3.0f;
			if (avionMovZ <= -300.0f) {
				std::cout << "Termina el estado 0" << std::endl;
				estadoAvion = 1;
			}
		}
		if (estadoAvion == 1) {
			avionMovY += 3.0f;
			avionMovZ -= 3.0f * 0.75;
			orientacionAvionX == -30.0f;

			if (avionMovY >= 200.0f) {
				estadoAvion = 2;
			}
		}
		if (estadoAvion == 2) {
			anguloAvion -= 0.01;
			avionMovX = 200 * cos(anguloAvion) + 145;
			avionMovZ = sqrt((200 * 55) - (cos(anguloAvion) * cos(anguloAvion))) * sin(anguloAvion) - 450.75;
			orientacionAvionY = -90.0f;
			if (anguloAvion <= -3.15) {
				estadoAvion = 3;
			}
		}

		if (estadoAvion == 3) {
			avionMovZ += 3.0;
			orientacionAvionY = 0.0f;
			if (avionMovZ >= 100.0f) {
				estadoAvion = 4;
			}
		}

		if (estadoAvion == 4) {
			anguloAvion -= 0.01;
			avionMovX = 200 * cos(anguloAvion) + 147;
			avionMovZ = sqrt((200 * 55) - (cos(anguloAvion) * cos(anguloAvion))) * sin(anguloAvion) + 100;
			orientacionAvionY = -270.0f;
			if (anguloAvion <= -6.30) {
				std::cout << "Termina Estado 4:" << std::endl;
				estadoAvion = 5;
				anguloAvion == 0.0f;

			}

		}

		if (estadoAvion == 5) {
			avionMovY -= 3.0f;
			avionMovZ -= 3.0f * 0.80;
			orientacionAvionY = -180.0f;


			if (avionMovY <= 0.1f) {
				estadoAvion = 6;
				std::cout << "Termina Estado 5: Coord Z" << avionMovZ << "Coord Y" << avionMovY << std::endl;
			}
		}

		if (estadoAvion == 6) {
			avionMovZ -= 3.0f;

			if (avionMovZ <= 150.0f) {
				estadoAvion = 0;
			}
		}
	}

	if (animacion)
	{
		if (circuito_auto == 0)
		{
			orienta = 0.0f;
			movAuto_z += 3.0f;
			orienta = 0.0f;
			if (movAuto_z >= 140.0)
				circuito_auto = 1;
		}
		if (circuito_auto == 1)
		{
			orienta = 270.0f;
			movAuto_x += -3.0f;
			if (movAuto_x <= -150.0)
				circuito_auto = 2;
		}
		if (circuito_auto == 2)
		{
			movAuto_z -= 3.0f;
			orienta = 180.0f;
			if (movAuto_z <= 120.0)
				circuito_auto = 3;
		}

		if (circuito_auto == 3)
		{
			movAuto_x += 3.0f;
			orienta = 90.0f;
			if (movAuto_x >= -35.0)
				circuito_auto = 4;
		}

		if (circuito_auto == 4)
		{
			movAuto_x -= (0.65f * 1.0f);
			movAuto_z -= 3.0f;
			orienta = 205.0f;
			if (movAuto_z <= -10.0)
				circuito_auto = 5;
		}

		if (circuito_auto == 5)
		{
			movAuto_z -= 3.0f;
			orienta = 180.0f;
			if (movAuto_z <= -20.0)
				circuito_auto = 6;
		}

		if (circuito_auto == 6)
		{
			movAuto_x -= 3.0f;
			orienta = 270.0f;
			if (movAuto_x <= -150.0)
				circuito_auto = 7;
		}

		if (circuito_auto == 7)
		{
			orienta = 0.0f;
			movAuto_z += 3.0f;
			if (movAuto_z > 100.0)
				circuito_auto = 8;
		}

		if (circuito_auto == 8)
		{
			movAuto_x += (3.56f * 1.0f);
			movAuto_z += -3.0f;
			orienta = -245.0f; //-147.995
			if (movAuto_z <= 65.0)
				circuito_auto = 9;
		}

		if (circuito_auto == 9)
		{
			movAuto_x -= (0.65f * 1.0f);
			movAuto_z -= 3.0f;
			orienta = 205.0f; //-147.995
			if (movAuto_z <= -10.0)
				circuito_auto = 10;
		}

		if (circuito_auto == 10)
		{
			movAuto_z -= 3.0f;
			orienta = 180.0f; //-147.995
			if (movAuto_z <= -30.0)
				circuito_auto = 11;
		}

		if (circuito_auto == 11)
		{
			movAuto_x += 3.0f;
			orienta = 90.0f; //-147.995
			if (movAuto_x >= 0.0)
				circuito_auto = 0;
		}

		if (circuito_auto == 13)
		{
			movAuto_x += 3.0f;
			orienta = 205.0f; //-147.995
			if (movAuto_x <= 0.0)
				movAuto_x = 0.0f;
			movAuto_z = 0.0f;
			circuito_auto = 0;
		}

	}

	if (!anim)
	{
		if (recorrido == 0)
		{
			movP_x -= 0.5f;
			giro = 270.0f;
			if (movP_x <= -150.0)
				recorrido = 1;

			//movP_z -= 1.0f;
			//giro = 180.0f;
			//if (movP_z <= -100.0)
				//recorrido = 1;
			//std::cout << "1 Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}

		if (recorrido == 1)
		{
			movP_x += 0.5f;
			giro = 90.0f;
			if (movP_x >= 0.0)
				recorrido = 0;
			/*movP_x -= 0.5f;
			giro = 270.0f;
			if (movP_x <= -125.0)
				recorrido = 2;
			//std::cout << "2 Coord Z" << movP_z << "Coord X" << movP_x << std::endl; */
		}
		
		if (recorrido == 2)
		{
			movP_x += 0.5f;
			giro = 90.0f;
			if (movP_x >= 0.0)
				recorrido = 1;
			
			//std::cout << "3 Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}
		/*
		if (recorrido == 3)
		{
			movP_z += 0.5f;
			giro = 0.0f;
			if (movP_z >= 0.0)
				recorrido = 2;
			//std::cout << "3 Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}
		
		if (recorrido == 2)
		{
			movP_z -= 0.5f;
			giro = 180.0f;
			if (movP_z <= -170.0)
				recorrido = 3;
			//std::cout << "3 Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}

		if (recorrido == 3)
		{
			movP_x += 0.5f;
			giro = 90.0f;
			if (movP_x >= 90.0)
				recorrido = 4;
			//std::cout << "4 Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}

		if (recorrido == 4)
		{
			movP_z += 0.5f;
			giro = 0.0f;
			if (movP_z >= -100.0)
				recorrido = 5;
			//std::cout << "Coord Z" << movP_z << "Coord X" << movP_x << std::endl;
		}

		if (recorrido == 5)
		{
			movP_x -= 0.5f;
			giro = 270.0f;
			if (movP_x == 0.0)
				recorrido = 1;
		} */
	}

	

	
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC 20241", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//irrklang::ISoundEngine* motor = irrklang::createIrrKlangDevice();

	//motor->play2D("media/music.mp3", true);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);

	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/derecha.jpg",
		"resources/skybox/izquierda.jpg",
		"resources/skybox/arriba.jpg",
		"resources/skybox/debajo.jpg",
		"resources/skybox/frente.jpg",
		"resources/skybox/detras.jpg"
	};

	Skybox skybox = Skybox(faces);

	// configuracion de shader
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// carga de modelos

	Model piso("resources/objects/piso/piso.obj");
	Model mapa("resources/objects/piso/mapa.obj");
	Model pisoArbustos("resources/objects/piso/pisoArbustos.obj");
	Model pisoCercas("resources/objects/piso/pisoCercas.obj");
	Model pisoMesas("resources/objects/piso/pisoMesas.obj");
	Model recepcion("resources/objects/piso/recepcion.obj");

	Model avionAmarillo("resources/objects/avionAmarillo/avionAmarillo.obj");
	
	Model Cadillac("resources/objects/Cadillac/cadilla.obj");
	Model Auto("resources/objects/auto/auto.obj");

	
	//ModelosProyecto
	Model mesaComida("resources/objects/mesaComida/fast_food_table__3d.obj");
	Model decoJardin("resources/objects/decoJardin/decorations_an.obj");
	Model childrenSlide("resources/objects/childrenSlide/children_s_slide.obj");
	Model playG("resources/objects/playG/playG.obj");
	Model silder("resources/objects/playG/slider/silder.obj");
	
	Model cafe("resources/objects/cafe/cafe.obj"); 
	Model pet("resources/objects/pet/_pet_shop_min.obj"); 
	Model donas("resources/objects/donas/donas.obj");
	Model pasillo("resources/objects/pasillo/pasillo.obj"); 
	Model ropaT("resources/objects/ropaT/ropaT.obj");
	Model bershka("resources/objects/bershka/bershka.obj");
	Model est("resources/objects/est/est.obj"); 
	Model telcel("resources/objects/telcel/telcel.obj");
	Model coppel("resources/objects/coppel/coppel.obj"); 
	Model macdonal("resources/objects/mcdonals/mcdonals.obj"); 

	
	Model lib("resources/objects/libreria/libreria.obj");
	
	Model bbva("resources/objects/bbva/bbva.obj");



	//PASTO

	Model pastoJuegos("resources/objects/areaJuegos/pastoJuegos.obj");
	Model palmera("resources/objects/areaJuegos/palmera/free_palm.obj");
	Model pastoFinal("resources/objects/areaJuegos/pastoFinal/pastoJ.obj");
	Model bancaMadera("resources/objects/areaJuegos/bancas/_wooden_benches__3d_.obj");
	Model bancaDos("resources/objects/bancaDos/_iron_bench__3d_asse.obj");


	//ModelAnim animacionPersonaje("resources/objects/Walking/Walking.dae");
	//animacionPersonaje.initShaders(animShader.ID);


	//Animaciones
	ModelAnim amyDance("resources/objects/Amy/Amy.dae");
	amyDance.initShaders(animShader.ID); //vacia la información en una estructura del objeto animado 

	ModelAnim jump("resources/objects/ninos/Jump.dae");
	jump.initShaders(animShader.ID);  

	ModelAnim seniora("resources/objects/seniora/seniora.dae");
	seniora.initShaders(animShader.ID);

	ModelAnim camina("resources/objects/seniora/camina/camina.dae");
	camina.initShaders(animShader.ID); 

	//Modelos Opel
	
	/* Model OpelCarr("resources/objects/opelGt/carro.obj");
	Model OpelPuertaDer("resources/objects/opelGt/puertaDer.obj");
	Model OpelPuertaIzq("resources/objects/opelGt/puertaIzq.obj");
	Model OpelRueda("resources/objects/opelGt/ruedaIzq.obj");
	Model OpelFaro("resources/objects/opelGt/faroDer.obj");
	Model OpelCapo("resources/objects/opelGt/capo.obj"); */


	//Inicializaci�n de KeyFrames

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posOpelX=0;
		KeyFrame[i].posOpelY=0;
		KeyFrame[i].posOpelZ=0;
		KeyFrame[i].rotOpelRueda=0;
		KeyFrame[i].rotOpelY=0;
		KeyFrame[i].rotOpelFaro =0;
		KeyFrame[i].rotOpelPuerta=0;
	}

	KeyFrame[0].posOpelX = 0;
	KeyFrame[0].posOpelY = 0;
	KeyFrame[0].posOpelZ = 0;
	KeyFrame[0].rotOpelRueda = 0;
	KeyFrame[0].rotOpelY = 0;
	KeyFrame[0].rotOpelFaro = 0;
	KeyFrame[0].rotOpelPuerta = 0;

	KeyFrame[1].posOpelX = 0;
	KeyFrame[1].posOpelY = 0;
	KeyFrame[1].posOpelZ = 0;
	KeyFrame[1].rotOpelRueda = 0;
	KeyFrame[1].rotOpelY = 0;
	KeyFrame[1].rotOpelFaro = 0;
	KeyFrame[1].rotOpelPuerta = 0;

	KeyFrame[2].posOpelX = 0;
	KeyFrame[2].posOpelY = 0;
	KeyFrame[2].posOpelZ = 0;
	KeyFrame[2].rotOpelRueda = 0;
	KeyFrame[2].rotOpelY = 0;
	KeyFrame[2].rotOpelFaro = 0;
	KeyFrame[2].rotOpelPuerta = 0;

	KeyFrame[0].posOpelX = 0;
	KeyFrame[0].posOpelY = 0;
	KeyFrame[0].posOpelZ = 0;
	KeyFrame[0].rotOpelRueda = 0;
	KeyFrame[0].rotOpelY = 0;
	KeyFrame[0].rotOpelFaro = 0;
	KeyFrame[0].rotOpelPuerta = 0;


	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		lastFrame = SDL_GetTicks();

		animate();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		staticShader.use();

		//staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //direccion de rayos luminosos
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.0f)); // Componente de luz ambiental
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // Componente de luz difusa
		staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f)); /**/

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", light);
		staticShader.setVec3("pointLight[0].diffuse", light);
		staticShader.setVec3("pointLight[0].specular", light);
		staticShader.setFloat("pointLight[0].constant", 0.08f);//variables para atenuacion 
		staticShader.setFloat("pointLight[0].linear", 0.00009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);//disminuir el no aumenta la intencidad
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", l);
		staticShader.setVec3("pointLight[1].diffuse", l);
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.01f);
		staticShader.setFloat("pointLight[1].linear", 0.0009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 0.01f);
		staticShader.setFloat("pointLight[2].linear", 0.0009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.0032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f))); //apertura
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(10.0f))); //difuminacion
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.00099f);
		staticShader.setFloat("spotLight[0].quadratic", 0.00005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		
		model = glm::translate(glm::mat4(1.0), glm::vec3(-300.0 - 800.0f, 20.0f, 25.0 - 170.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		animShader.setMat4("model", model);
		amyDance.Draw(animShader);

		/* model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f + avionMovX, 0.0f + avionMovY, 0.0f + avionMovZ));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + movP_x, 0.0f, -350.0f + movP_z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader); */

		//------------------------------------Modelos Animados -------------------------------------------//
		//---------------------------------------------------------------------------------------------------//
		model = glm::translate(glm::mat4(1.0), glm::vec3(-500.0 - 800.0f, 20.0f, 25.0 - 200.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		animShader.setMat4("model", model);
		jump.Draw(animShader);


		model = glm::translate(glm::mat4(1.0), glm::vec3(-650.0 - 800.0f, 8.0f, 0.0 - 300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.28f));
		animShader.setMat4("model", model);
		seniora.Draw(animShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f + movP_x, 0.0f, -450.0f + movP_z));  // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		camina.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f + movP_x, 0.0f, -650.0f + movP_z));  // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(giro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		camina.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f - movP_x, 0.0f, -450.0f - movP_z));  // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-giro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		camina.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f - movP_x, 0.0f, -650.0f - movP_z));  // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-giro), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		camina.Draw(animShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		mapa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		pisoArbustos.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		pisoCercas.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		pisoMesas.Draw(staticShader);
	

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		recepcion.Draw(staticShader);



	// vehiculos
	// -------------------------------------------------------------------------------------------------------------------------
	
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1000.0f + movAuto_x, 0.0f, 800.0f + movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.9f));
		staticShader.setMat4("model", model);
		Auto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(900.0f, 0.0f, 750.0f ));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Cadillac.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f + avionMovX, 0.0f + avionMovY, 0.0f + avionMovZ));
		model = glm::rotate(model, glm::radians(orientacionAvionY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientacionAvionX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		avionAmarillo.Draw(staticShader);
		


		//---------------------------------Modelos Cynthia ------------------------------/////
		//----------------------------------------------------------------------------------
		//Escenario para el area de juegos.


		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0 - 800.0f, 2.75f, 0.0 - 500.0f));
		model = glm::scale(model, glm::vec3(15.f, 0.0f, 10.0f));
		staticShader.setMat4("model", model);
		pastoFinal.Draw(staticShader);


		//modelo3 childrenSlide
		model = glm::translate(glm::mat4(1.0), glm::vec3(-200.0 - 800.0f, 30.0f, 25.0 - 300.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		childrenSlide.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-200.0 - 800.0f, 30.0f, 25.0 - 350.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		childrenSlide.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-450.0 - 800.0f, 20.0f, 25.0 - 200.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		playG.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-600.0 - 800.0f, 20.0f, 25.0 - 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		silder.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0), glm::vec3(-650.0 - 800.0f, 20.0f, 0.0 - 300.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-650.0 - 800.0f, 20.0f, 150.0 - 300.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-300.0 - 800.0f, 4.75f, -200.0 - 300.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-300.0 - 800.0f, 4.75f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0), glm::vec3(-650.0 - 800.0f, 4.75f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);


		//-------------------------------------- Escenario tiendas izquierda ------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0), glm::vec3(-170.0f - 200.0f, 60.0f, -150.0f - 250.0f));
		model = glm::scale(model, glm::vec3(130.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pet.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0 - 750.0f, 0.0f, -500.0 - 400.0f));
		model = glm::scale(model, glm::vec3(1.20f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		donas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0 - 750.0f, 0.0f, -680.0 - 400.0f));
		model = glm::scale(model, glm::vec3(1.30f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cafe.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0f - 750.0f, 0.0f, -400.0f - 300.0f));
		model = glm::scale(model, glm::vec3(1.30f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		macdonal.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0f - 250.0f, 0.0f, -680.0f - 450.0f));
		model = glm::scale(model, glm::vec3(1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bbva.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0f - 250.0f, 0.0f, -250.0f - 450.0f));
		model = glm::scale(model, glm::vec3(1.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lib.Draw(staticShader);

		//----------------------------------- Escenario zona descanso ------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------//

		model = glm::translate(glm::mat4(1.0), glm::vec3(-550.0 - 300.0f, 10.0f, 250.0 - 500.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-550.0 - 300.0f, 10.0f, 250.0 - 400.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-550.0 - 300.0f, 10.0f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-550.0 - 300.0f, 10.0f, 250.0 - 600.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-550.0 - 300.0f, 10.0f, 250.0 - 700.0f));
		model = glm::scale(model, glm::vec3(40.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaMadera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-350.0 - 300.0f, 10.0f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(60.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaDos.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0), glm::vec3(-350.0 - 200.0f, 10.0f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(60.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaDos.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-350.0 - 100.0f, 10.0f, 250.0 - 300.0f));
		model = glm::scale(model, glm::vec3(60.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bancaDos.Draw(staticShader);

		//-----------------------------------Escenario tiendas enmedio -------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------//
		model = glm::translate(glm::mat4(1.0), glm::vec3(-30.0f -200.0f, 0.0f, -130.0f -300.0f));
		model = glm::scale(model, glm::vec3(0.45f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ropaT.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-30.0f - 200.0f, 0.0f, -230.0f - 450.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bershka.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(30.0f + 200.0f, 0.0f, -230.0f - 450.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		telcel.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(30.0f + 200.0f, 0.0f, -130.0f - 300.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		coppel.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0), glm::vec3(-0.60, 0.0f, -450.0 - 100.0f));
		model = glm::scale(model, glm::vec3(5.5f, 0.0f, 5.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pasillo.Draw(staticShader);



		//Escenario estacionamiento 

		model = glm::translate(glm::mat4(1.0), glm::vec3(500.0f, 0.0f, -650.0 - 100.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		est.Draw(staticShader);


	
		// Termina Escenario
	

		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);


		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop

		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}


void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	/*if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;*/
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		
		light = glm::vec3(1.0f, 0.0f, 0.0f);
		on = true;
		
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		// Si la tecla 'L P' está presionada, cambia el estado de la luz
		if (o) {
			l = glm::vec3(0.0f, 0.0f, 0.0f);  // Apaga la luz (color negro)
			o = false;
		}
		else {
			l = glm::vec3(1.0f, 1.0f, 1.0f);  // Enciende la luz (color blanco)
			o = true;
		}
	}

	

	/*if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		light = glm::vec3((-30.0f - 200.0f, 0.0f, -130.0f - 300.0f));
		
		light = glm::vec3((-30.0f - 200.0f, 0.0f, -230.0f - 450.0f));
		

		light = glm::vec3((30.0f + 200.0f, 0.0f, -230.0f - 450.0f));
		

		light = glm::vec3((30.0f + 200.0f, 0.0f, -130.0f - 300.0f));
		
		on = false;
	}*/

	

	/*if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		light = glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		light = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		light = glm::vec3(0.0f, 0.0f, 1.0f);*/

		//animacion
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		circuito_auto = 0;
		animacion = true;

	}


	 if(key == GLFW_KEY_Z && action == GLFW_PRESS)
	{

		//recorrido = 0;
		//!anim = anim;

	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		//animacion = false;
		//recorrido = 0;
		//anim = !anim;

	}


	//Car animation
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		//animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
