tensor = load_nii('C:/ETH/Neuro/GlobalTracking/1159_tensor.nii');

dim = size(tensor.img);

[tensorfile,message] = fopen('tensor_data.txt','w');

z1 = 48;
z2 = 64;
z3 = 64;


fprintf(tensorfile,'{');
for i = z1-4:z1+4
    fprintf(tensorfile,'{');
    for j = z2-4:z2+4
        fprintf(tensorfile,'{');
        for k = z3-4:z3+4
            fprintf(tensorfile,'{');
            for t = 1:6
                if t < 6
                fprintf(tensorfile,'%f,',tensor.img(i,j,k,t));
                else
                fprintf(tensorfile,'%f',tensor.img(i,j,k,t));
                end
            end
            if k < z3+4
            fprintf(tensorfile,'},');
            else
            fprintf(tensorfile,'}');
            end
        end
        if j < z2+4
        fprintf(tensorfile,'},');
        else
        fprintf(tensorfile,'}');
        end
    end
    if i < z1+4
    fprintf(tensorfile,'},');
    else
    fprintf(tensorfile,'}');
    end
end
fprintf(tensorfile,'}');

fclose(tensorfile);