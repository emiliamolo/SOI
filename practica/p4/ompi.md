# OMPI

# MPI_Scatter: Dividir una estructura en porciones y distribuir esas porciones a otros procesos
MPI_Scatter(
    sendbuf,     // puntero al buffer a repartir (sólo relevante en root)
    sendcount,   // cantidad de elementos enviados a cada proceso
    sendtype,    // tipo de dato enviado
    recvbuf,     // buffer local donde cada proceso recibe sus datos
    recvcount,   // cantidad de elementos que recibe cada proceso
    recvtype,    // tipo de dato recibido
    root,        // proceso root que reparte
    comm         // comunicador (por lo general MPI_COMM_WORLD)
);

Es necesario que sencount * numero de procesos  = size(sendbuf)
Para solucionar eso podemos usar MPI_Scatterv
MPI_Scatterv(
    sendbuf,        // buffer completo (en root)
    sendcounts,     // arreglo con cantidad de elementos para cada proceso
    displs,         // arreglo con desplazamientos (offsets) en sendbuf para cada proceso
    sendtype,
    recvbuf,        // buffer local para cada proceso
    recvcount,      // cantidad que espera recibir este proceso
    recvtype,
    root,
    comm
);

MPI_Gatherv(sendbuf, sendcount, sendtype,
            recvbuf, recvcounts, displs, recvtype,
            root, comm);
Solo el proceso root necesita recvbuf, recvcounts, y displs.