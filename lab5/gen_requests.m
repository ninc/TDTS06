function req = gen_request()

    % PROGRAM PARAMETERS
    % Time
    number_of_time_stamps = 1000;
    request_rate = 10;
    max_time_stamp = 50000;
    
    % Files
    number_of_files = 10;
    max_video_duration = 100;
    max_bytes = 1000;
    % Servers
    number_of_servers = 5;
    % Clients
    number_of_clients = 5;
    
    
    %--------------------------------
    % TIME STAMP
    % Generate uniform distribution
    u = rand(number_of_time_stamps, 1);
    
    % Distribute requests according to instructions
    x = (1/request_rate)*(log(u));
    
    % Sort the requests
    x = sort(abs(x));
    
    % Apply constant
    time_stamp = round(max_time_stamp*x)+1;
    %--------------------------------
    
    
    % Create servers
    for j=1:number_of_servers,
       servers(j) = cellstr(['server' int2str(j)]);
    end
    
    % Create files
    for j=1:number_of_files,
       files(j) = cellstr(['file' int2str(j)]);
       % Uniform distribution
       video_duration(j) = round(rand(1)*max_video_duration);
       file_bytes(j) = round(rand(1)*max_bytes);
    end
    

    
    % Create clients
    for j=1:number_of_clients,
       clients(j) = cellstr(['client' int2str(j)]);    
    end
    
    % Create a ZIPF distribution of when someone access what file using randraw.m
    %Y = randraw('zeta', number_of_files, number_of_time_stamps);
    
    
    
    %disp(clients);
    %disp(servers);
    %disp(files);
    %disp(video_duration);
    %disp(file_bytes);
    %disp(Y);
    
    fileID = fopen('gen_input.txt','w');
    
    
    %Write to file
    for j=1:number_of_time_stamps,
        time = time_stamp(j);
        %Uniform distribution of clients and servers
        c = round(rand(1)*number_of_clients);
        s = round(rand(1)*number_of_servers);
        
        f =  round(rand(1)*number_of_files);
        
        
        if(c == 0)
            c = 1;
        end
        if(s == 0)
            s = 1;
        end
        if(f == 0)
            f = 1;
        end
        
        client = clients{c};
        server = servers{s};
        %ZIPF of file used
        video_dur = video_duration(f);
        file = files{f};
        file_b = file_bytes(f);
        
        %Write to file
        fprintf(fileID, '%i ', time);
        fprintf(fileID, '%s ', client);
        fprintf(fileID, '%i ', video_dur);
        fprintf(fileID, '%s ', server);
        fprintf(fileID, '%s ', file);
        fprintf(fileID, '%i\r\n', file_b);
        
        
        
    end
    
    fclose(fileID);
    
    req = 'File created';
end