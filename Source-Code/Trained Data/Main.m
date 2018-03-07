input = importdata('avgData.txt');

plot(input);
hold on;

title('Evolution of the Weights during training');
xlabel('Step');
ylabel('Weight Value');