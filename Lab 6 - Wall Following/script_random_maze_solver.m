%% ME 545 Lab 6
% Group 5: Juliette Mitrovich, Sheila Moroney, Sujani Patel
% Maze solving
clc
clear
close all
%%
maze = zeros(7);
for i = 2:(size(maze)-1)
    for j = 2:(size(maze)-1)
        maze(i,j) = round(rand);
    end
end
maze(2,6:7) = 0.5;
maze(6,2) = 0.5;
% imshow(maze,'InitialMagnification',20000);

% try to solve the maze
direction = 0; % way the robot should move (0,90,180,270)
robot_spd = 1; % how fast the robot moves
compass = [-sind(direction), cosd(direction)]; % convert direction to north, south, east, west
location = [6,2];
laps = 1;
laps_prev = laps;
moved = 0;
solve = 0;
sum_mazes = 0;
sum_time = 0;
while solve == 0
    direction = direction - 90;
    compass = [-sind(direction), cosd(direction)]; % look right
    check = location + compass; % location of square you're looking at
    % if there's no empty square, move 90º and check again
    if maze(check(1),check(2)) == 0
        direction = direction + 90;
        compass = [-sind(direction), cosd(direction)];
        check = location + compass;
        if maze(check(1),check(2)) == 0
            direction = direction + 90;
            compass = [-sind(direction), cosd(direction)];
            check = location + compass;

            if maze(check(1),check(2)) == 0
                direction = direction + 90;
                compass = [-sind(direction), cosd(direction)];
                check = location + compass;
            end
        end
    end
    % if there's an empty square move to it
    if maze(check(1),check(2)) > 0
        location = check;
        if laps_prev ~= laps
            moved = 0;
            laps_prev = laps;
        end
        moved = moved + 1;
        maze(location(1),location(2)) = 0.6;

        %         figure(laps)
        %         imshow(maze,'InitialMagnification',20000)
        maze(location(1),location(2)) = 1;
    end
    % if you arrive at the start again, generate a new maze
    if location == [6,2]
        maze = zeros(7);
        for i = 2:(size(maze)-1)
            for j = 2:(size(maze)-1)
                maze(i,j) = round(rand);
            end
        end
        maze(2,6:7) = 0.5;
        maze(6,2) = 0.5;
        laps = laps + 1;
        %         close all
        %         figure(laps)
        %         imshow(maze,'InitialMagnification',20000);
        %         fprintf('REDOING MAP\n')
    end
    %     pause(0.25);
    % If you complete the maze, keep generating mazes until you've generated
    % every possible one
    if location == [2,7]
        maze = zeros(7);
        for i = 2:(size(maze)-1)
            for j = 2:(size(maze)-1)
                maze(i,j) = round(rand);
            end
        end
        maze(2,6:7) = 0.5;
        maze(6,2) = 0.5;
        location = [6,2];
        laps = laps + 1;
        %         close all
        %         figure(laps)
        %         imshow(maze,'InitialMagnification',20000);
        %         fprintf('REDOING MAP\n')
        sum_mazes = sum_mazes + 1;
        sum_time = sum_time + moved*robot_spd;
        %         fprintf('REDOING MAP\n')
        %         fprintf('\n \n \nGood Job! Your robot solved it in %f seconds \n',moved*robot_spd)
    end

    if laps == 2^23
        solve = 1;
        fprintf('\n \n \nGood Job! Your average time was %0.2f seconds \n',sum_time/sum_mazes)
    end
end
