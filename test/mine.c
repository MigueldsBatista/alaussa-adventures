#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

// Função para carregar uma textura a partir de um arquivo de imagem
SDL_Texture* loadTexture(const char* file, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erro ao criar textura: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

// Função para renderizar o fundo com efeito parallax
void renderParallax(SDL_Renderer* renderer, SDL_Texture* background, int camera_position, int screen_width, int screen_height) {
    int bg_width = 725;  // Largura da imagem de fundo
    int bg_height = 400; // Altura da imagem de fundo

    // Calcula o deslocamento do parallax (horizontal)
    int parallax_offset = camera_position % bg_width;

    // Renderiza a primeira parte da imagem
    SDL_Rect src_rect1 = { parallax_offset, 0, bg_width - parallax_offset, bg_height };
    SDL_Rect dst_rect1 = { 0, 0, bg_width - parallax_offset, bg_height };
    SDL_RenderCopy(renderer, background, &src_rect1, &dst_rect1);

    // Se houver espaço sobrando, renderiza a segunda parte da imagem para "repeti-la"
    if (screen_width > (bg_width - parallax_offset)) {
        SDL_Rect src_rect2 = { 0, 0, parallax_offset, bg_height };
        SDL_Rect dst_rect2 = { bg_width - parallax_offset, 0, parallax_offset, bg_height };
        SDL_RenderCopy(renderer, background, &src_rect2, &dst_rect2);
    }
}

int main(int argc, char* argv[]) {
    // Inicializa SDL e SDL_image
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erro ao inicializar SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Cria a janela
    SDL_Window* window = SDL_CreateWindow("Parallax Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Cria o renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Carrega a textura do fundo
    SDL_Texture* background = loadTexture("parallax.png", renderer);
    
    if (!background) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Loop principal
    int camera_position = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Limpa a tela
        SDL_RenderClear(renderer);

        // Renderiza o fundo com efeito de parallax
        renderParallax(renderer, background, camera_position, 640, 480);

        // Atualiza a tela
        SDL_RenderPresent(renderer);

        // Simula o movimento da câmera
        camera_position += 2;

        // Limita a taxa de quadros
        SDL_Delay(16);
    }

    // Limpa os recursos
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
    //gcc -o parallax_example main.c -lSDL2 -lSDL2_image

}
