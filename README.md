# 📋 Mapeamento Estratégico: Assistente de Sudoku (Qt/C++)

## 1. Core e Lógica de Dados (Model)

- [x] Estrutura do projeto CMake separando `src` e `tests`.
- [x] Representação do estado do jogo (matriz 9x9).
- [x] Integração do sistema de recursos do Qt (`assets.qrc`).
- [x] Lógica de parsing de string para matriz (`loadFromString`).
- [x] Motor matemático para calcular as possibilidades restantes de uma célula (`getPossibilities`).
- [x] Testes unitários validando a regra de negócios.
- [x] Carregar os outros arquivos de nível (Medium.txt, Hard.txt, Insane.txt) no `.qrc`.
- [x] **[CRÍTICO]** Sorteio aleatório da grade: Ler o número inicial (ex: 200) e pular para uma linha aleatória do arquivo a cada novo jogo.

## 2. Arquitetura (Critérios de Avaliação)

- [ ] Implementar o padrão **Observer**: O Model deve notificar mudanças de estado sem acoplamento direto com a View.
- [ ] Implementar o padrão **Model-View-Presenter (MVP)**: Criar a classe Presenter para intermediar cliques da View e regras do Model.

## 3. Interface Gráfica e Interação (View)

- [x] Janela principal renderizando os dados crus em um `QTableWidget`.
- [ ] Refinar o visual da grade: Engrossar as linhas separadoras dos quadrantes 3x3 e ajustar paleta de cores.
- [ ] Capturar a seleção de uma célula pelo usuário.
- [ ] Criar os controles de jogo: ComboBox de Nível ("Easy", "Medium", etc.) e botão "New Game".
- [ ] Interface de Inserção: Permitir que o usuário defina um número em uma célula vazia.

## 4. O Assistente & Inovações (Diferencial Competitivo)

- [ ] Exibir o "helper" (possibilidades calculadas): Apresentar os dados de forma elegante (ex: painel lateral dinâmico ou anotações na célula, rejeitando ativamente o layout confuso do PDF).
- [ ] Dica Visual 1: Destacar automaticamente células que possuem apenas **uma** possibilidade restante.
- [ ] Dica Visual 2: Alertar visualmente células travadas (conflito de regras / zero possibilidades).
- [ ] Ergonomia: Adicionar atalhos de teclado (navegação via setas direcionais, inserção via numpad).
- [ ] Ergonomia: Inserir _Tooltips_ (bulles d'aide) explicando o uso da interface.
- [ ] Internacionalização (i18n): Estruturar o código com `tr()` para suportar múltiplos idiomas.

## 5. Entrega Final

- [ ] Elaborar README ou relatório técnico (PDF) detalhando a arquitetura aplicada (MVP/Observer) e as decisões de UX/UI.
- [ ] Preparar a demonstração de 5 minutos.
