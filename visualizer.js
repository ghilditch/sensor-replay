Qt.include("three.js")

var camera, scene, renderer;

var total_body_parts = 9;
var body_part_cnt = 0;
var body_parts = {
    body: 0,
    upperArmL: 1,
    upperArmR: 2,
    lowerArmL: 3,
    lowerArmR: 4,
    upperLegL: 5,
    upperLegR: 6,
    lowerLegL: 7,
    lowerLegR: 8
}

// Models
var athelete = [null, null, null, null, null, null, null, null, null];

var ToRad = Math.PI / 180;
var qmlView;

function initializeGL(canvas, eventSource, mainView) {

    qmlView = mainView;

    renderer = new THREE.Canvas3DRenderer(
                { canvas: canvas, antialias: true, devicePixelRatio: canvas.devicePixelRatio });
    renderer.setSize(canvas.width, canvas.height);
    renderer.gammaInput = true;
    renderer.gammaOutput = true;
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.renderReverseSided = false;
    renderer.setClearColor(new THREE.Color("cyan"));

    scene = new THREE.Scene();
    scene.fog = new THREE.Fog( 0xffffff, 1, 5000 );
    scene.fog.color.setHSL( 0.6, 0, 1 );

    camera = new THREE.PerspectiveCamera( 80, canvas.width / canvas.height, 1, 5000 );
    camera.position.set( 0, 4, 0 );
    //camera.lookAt(scene.position);

    var hemiLight = new THREE.HemisphereLight( 0xffffff, 0xffffff, 0.25 );
    hemiLight.color.setHSL( 0.6, 1, 0.6 );
    hemiLight.groundColor.setHSL( 0.095, 1, 0.75 );
    hemiLight.position.set( 0, 500, 0 );
    scene.add( hemiLight );

    var dirLight = new THREE.DirectionalLight( 0xffffff, 1 );
    dirLight.color.setHSL( 0.1, 1, 0.95 );
    dirLight.position.set( -1, 1.75, 1 );
    dirLight.position.multiplyScalar( 50 );
    scene.add( dirLight );

    dirLight.castShadow = true;

    dirLight.shadow.mapSize.width = 2048;
    dirLight.shadow.mapSize.height = 2048;

    var d = 50;

    dirLight.shadow.camera.left = -d;
    dirLight.shadow.camera.right = d;
    dirLight.shadow.camera.top = d;
    dirLight.shadow.camera.bottom = -d;

    dirLight.shadow.camera.far = 3500;
    dirLight.shadow.bias = -0.0001;

    createGrid();

    loadAtheleteMeshes();

    //camera.lookAt ( scene.position );
}

function createGrid(){
    var size = 14;
    var step = 1;

    var geometry = new THREE.Geometry();
    var material = new THREE.LineBasicMaterial({color: 'white'});

    for (var i = -size; i <= size; i+=step){
        geometry.vertices.push(new THREE.Vector3(-size, -0.04, i));
        geometry.vertices.push(new THREE.Vector3(size, -0.04, i));

        geometry.vertices.push(new THREE.Vector3(i, -0.04, -size));
        geometry.vertices.push(new THREE.Vector3(i ,-0.04, size));
    }

    var lines = new THREE.Line(geometry, material, THREE.LinePieces);
    scene.add (lines);
}

function addMeshToScene(mesh){
    mesh.scale.set(10,10,10);
    mesh.position.set (0,0,0);
    //mesh.matrixAutoUpdate = false;
    //mesh.rotation.set(new THREE.Vector3( 0, 0, Math.PI / 2));
    //mesh.rotation.y = Math.PI / 2;
    //mesh.rotation.x = Math.PI / 4;
    scene.add( mesh );
    // increment the body_parts
    body_part_cnt++;
    // Check if we are good to good
   if (isAtheleteLoaded()){
       onRotateCamera(0);
   }
}

function loadAtheleteMeshes(){
    var request = new XMLHttpRequest();
    request.open("GET", "models/body.json");
    request.onreadystatechange = function () {
        if (request.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request.responseText), addBodyToScene );
        }
    }
    request.send();

    var request2 = new XMLHttpRequest();
    request2.open("GET", "models/upperArm.L.json");
    request2.onreadystatechange = function () {
        if (request2.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request2.responseText), addupperArmLToScene );
        }
    }
    request2.send();

    var request3 = new XMLHttpRequest();
    request3.open("GET", "models/upperArm.R.json");
    request3.onreadystatechange = function () {
        if (request3.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request3.responseText), addupperArmRToScene );
        }
    }
    request3.send();

    var request4 = new XMLHttpRequest();
    request4.open("GET", "models/lowerArm.L.json");
    request4.onreadystatechange = function () {
        if (request4.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request4.responseText), addlowerArmLToScene );
        }
    }
    request4.send();

    var request5 = new XMLHttpRequest();
    request5.open("GET", "models/lowerArm.R.json");
    request5.onreadystatechange = function () {
        if (request5.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request5.responseText), addlowerArmRToScene );
        }
    }
    request5.send();

    var request6 = new XMLHttpRequest();
    request6.open("GET", "models/upperLeg.L.json");
    request6.onreadystatechange = function () {
        if (request6.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request6.responseText), addupperLegLToScene );
        }
    }
    request6.send();

    var request7 = new XMLHttpRequest();
    request7.open("GET", "models/upperLeg.R.json");
    request7.onreadystatechange = function () {
        if (request7.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request7.responseText), addupperLegRToScene );
        }
    }
    request7.send();

    var request8 = new XMLHttpRequest();
    request8.open("GET", "models/lowerLeg.L.json");
    request8.onreadystatechange = function () {
        if (request8.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request8.responseText), addlowerLegLToScene );
        }
    }
    request8.send();

    var request9 = new XMLHttpRequest();
    request9.open("GET", "models/lowerLeg.R.json");
    request9.onreadystatechange = function () {
        if (request9.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request9.responseText), addlowerLegRToScene );
        }
    }
    request9.send();
}

function onRotateCamera(value) {
    var rad = ToRad * value ;
    camera.position.x = Math.cos(rad) * 10;
    camera.position.z = Math.sin (rad) * 10;
    camera.lookAt ( scene.position );
}

function onSpeedChanged(value) {
    //daysPerFrameScale = value;
}

function setScale(value, focused) {

}

function setCameraDistance(distance) {
    //cameraDistance = distance;

}
function onResizeGL(canvas) {
    camera.aspect = canvas.width / canvas.height;
    camera.updateProjectionMatrix();

    renderer.setPixelRatio(canvas.devicePixelRatio);
    renderer.setSize(canvas.width, canvas.height);
}

function isAtheleteLoaded(){
    if( body_part_cnt === total_body_parts ){
        return true;
    }else{
        return false;
    }
}

function updateMovement(body_part, x, y, z, q1, q2, q3, q4){

    var pos = new THREE.Vector3(x, y, z)
    var quat = new THREE.Quaternion(q1, q2, q3, q4);

    //console.log("update body part ", body_part);

    athelete[body_part].position.copy(pos);
    athelete[body_part].quaternion.copy(quat);

    //console.log("updated body part ", body_part);
}

function paintGL(canvas) {

    //var timer = Date.now() * 0.0005;
    //camera.position.x = Math.cos(timer) * 10;
    //camera.position.z = Math.sin (timer) * 10;
    //camera.lookAt ( scene.position );

    //if( isAtheleteLoaded){
    //    moveAthelete();
    //}

    renderer.render( scene, camera );
}

function moveAthelete(){
    var _matrix, _matrix2;
    var pos = new THREE.Vector3(), quat = new THREE.Quaternion();

    for (var i = 0; i < body_part_cnt; i++){
        _matrix = new THREE.Matrix4();

        if (i === body_parts.upperLegR){
            _matrix.makeTranslation( 140, 20, 0);
            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationY(90*ToRad);
            _matrix.multiply( _matrix2 );

            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationX(-33*ToRad);
            _matrix.multiply( _matrix2 );
        }else if(i === body_parts.lowerLegR){
            _matrix.makeTranslation( 15, 5, 0);
            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationY(90*ToRad);
            _matrix.multiply( _matrix2 );

            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationX(30*ToRad);
            _matrix.multiply( _matrix2 );
        }else if(i === body_parts.lowerArmR){
            _matrix.makeTranslation( 15, 5, 0);
            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationY(90*ToRad);
            _matrix.multiply( _matrix2 );

            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationX(-180*ToRad);
            _matrix.multiply( _matrix2 );
        }else if(i === body_parts.upperArmR){
            _matrix.makeTranslation( 15, 5, 0);
            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationY(90*ToRad);
            _matrix.multiply( _matrix2 );

            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationX(-180*ToRad);
            _matrix.multiply( _matrix2 );
        }else{
            _matrix.makeTranslation( 0, 0, 0);
            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationY(90*ToRad);
            _matrix.multiply( _matrix2 );

            _matrix2 = new THREE.Matrix4();
            _matrix2.makeRotationX(10*ToRad);
            _matrix.multiply( _matrix2 );
        }

        pos.setFromMatrixPosition( _matrix );
        quat.setFromRotationMatrix( _matrix );
        athelete[i].position.copy(pos);
        athelete[i].quaternion.copy(quat);
    }
}

function addBodyToScene ( geometry, materials ) {
    athelete[body_parts.body] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.body] );
};

function addupperArmLToScene ( geometry, materials ) {
    athelete[body_parts.upperArmL] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.upperArmL] );
};

function addupperArmRToScene ( geometry, materials ) {
    athelete[body_parts.upperArmR] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.upperArmR] );
};

function addlowerArmLToScene ( geometry, materials ) {
    athelete[body_parts.lowerArmL] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.lowerArmL] );
};

function addlowerArmRToScene ( geometry, materials ) {
    athelete[body_parts.lowerArmR] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.lowerArmR] );
};

function addupperLegLToScene ( geometry, materials ) {
    athelete[body_parts.upperLegL] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.upperLegL] );
};

function addupperLegRToScene ( geometry, materials ) {
    athelete[body_parts.upperLegR] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.upperLegR] );
};

function addlowerLegLToScene ( geometry, materials ) {
    athelete[body_parts.lowerLegL] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.lowerLegL] );
};

function addlowerLegRToScene ( geometry, materials ) {
    athelete[body_parts.lowerLegR] = new THREE.Mesh( geometry, materials[0] );
    addMeshToScene ( athelete[body_parts.lowerLegR] );

};

