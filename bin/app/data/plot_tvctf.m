function [] = plot_tvctf(fileName)

[freqAxis timeAxis data] = load_ctf(fileName);
figure;
imagesc(freqAxis, timeAxis, abs(data));
colorbar;
xlabel('Frequency (Hz)');
ylabel('Time (s)');
