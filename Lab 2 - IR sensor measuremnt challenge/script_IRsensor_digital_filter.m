%% Task 5: Butter
close all;
clc
% Fill in the time vector, and create noisy data
time = 0:0.01:10;
u_noisy = 3*sin(time) + 2.5*randn([1,length(time)]);

% This is the command for 25 cutoff frequency and 100 sampling frequency:
[b,a]= butter(1,14.7/166.667);

% The filter command can do the filtering
filter_command_result = filter(b,a,u_noisy);
array_y_filt = zeros(length(u_noisy));

% Now we can simulate the "loop" function in the Arduino to do the same
% thing as the filter command. 
old_y_filt = 0; % Initialize the first output
old_measurement = u_noisy(1); % Initialize the first input
array_y_filt(1) = 0;
for i = 2:length(u_noisy)
    measurement = u_noisy(i);
    y_filt= 0.2452*measurement + 0.2452*old_measurement+0.5095*old_y_filt;
    % Print this to the serial monitor
    old_y_filt = y_filt;
    old_measurement = measurement;
    % Here's where you would wait for the clock to "tick" up by 0.01
    % seconds.
    array_y_filt(i) = y_filt;
end
%%
% Plot the result
h = figure(1);
clf;
hold on;
grid minor
set(h,'Name','Hand code results');
plot(time,u_noisy,'r',time,array_y_filt,'g','Linewidth',2);
xlabel('Time [sec]');
ylabel('Output [volts]');

h = figure(2);
clf;
hold on;
grid minor
set(h,'Name','Using the filter command');
plot(time,u_noisy,'r',time,filter_command_result,'b','Linewidth',2);
xlabel('Time [sec]');
ylabel('Output [volts]');
