# Terminal_de_login_digital
Projeto final da matéria de sistemas micrprocessados

Projeto: Sistema de segurança com senha de 1 a 4 dígitos.
O nosso projeto se baseia em usar um teclado matricial 4x3 para ser o painel de senha, um TM1637
4-digit-7segment led para exibir os números digitados, além de informa se a senha fornecida está correta ( open - indicando a abertura da tranca, 
done - indicando que a senha foi salva, erro - indicando que a operação é inválida).
O funcionamento se dá da seguinte forma, o usuário pode adicionar um número limitado de senhas 
( no máximo 8, contudo é possível aumentar ), ele pode desbloquear digitando uma combinação no teclado matricial 4x3,
que ao apertar o botão ' * ' ele envia a senha, retornando erro caso a senha esteja incorreta e ' # ' ele salva a senha,
retornando erro caso a senha já esteja cadastrada. Além foi criado um log para monitorar as tentativas de login e salvamento
de senhas, sendo usado uma rs232 para comunicar o pc e o stm32, foi usado o aplicativo Serial Port Monitor para monitorar os logs,
ademais, as tentativas de Log-in ficam salvas no arquivo gerado pelo Serial Port Monitor, como anexado em Logs.txt

OBSERVAÇÃO: O nome do projeto diverge do presente no arquivo, contudo é apenas no nome que ele diverge já que os códigos para a execução
das tarefas descritas acima estão presentes no mesmo


Gabriel dos Reis Rodrigues - 508342
João Amauri Rodrigues Do Nascimento - 510483 
João Lucas Oliveira Mota - 509597 

LINK PARA O VÍDEO: https://youtu.be/5nuqsLzA48E
