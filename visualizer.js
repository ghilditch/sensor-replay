Qt.include("three.js")

var camera, scene, renderer;

var total_body_parts = 1;
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
var bone_helper;

var ToRad = Math.PI / 180;
var qmlView;

// Simulate movement of the legs
var maxLegAngle = 95;
var minLegAngle = 45;
var currAngle = minLegAngle;
var currDirectionUp = true;

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

    camera = new THREE.PerspectiveCamera( 60, canvas.width / canvas.height, 1, 1000 );
    //camera.position.set( 0, 4, 0 );
    //camera.lookAt(scene.position);
    moveCamera(-45, 110 , -5);

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

    loadBikerMesh();
}

var moveCamera = function (h,v,d) {
    var center = {x:0, y:0, z:0};
    camera.position.copy(orbit(center, h, v, d));
    camera.lookAt(center);
}

function orbit(origine, h, v, distance) {
    origine = origine || new THREE.Vector3();
    var p = new THREE.Vector3();
    var phi = v*Math.PI / 180;
    var theta = h*Math.PI / 180;
    p.x = (distance * Math.sin(phi) * Math.cos(theta)) + origine.x;
    p.z = (distance * Math.sin(phi) * Math.sin(theta)) + origine.z;
    p.y = (distance * Math.cos(phi)) + origine.y;
    return p;
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

function loadBikerMesh(){
    var request = new XMLHttpRequest();
    request.open("GET", "models/biker.json");
    request.onreadystatechange = function () {
        if (request.readyState === XMLHttpRequest.DONE) {
            var loader = new THREE.JSONLoader();
            loader.load( JSON.parse(request.responseText), addBikerToScene );
        }
    }
    request.send();
}

function addBikerToScene ( geometry, materials ) {
    materials.forEach (function (mat){
        mat.skinning = true;
      });
    athelete[body_parts.body] = new THREE.SkinnedMesh( geometry, materials[0] );
    athelete[body_parts.body].rotation.x = 90 * ToRad;
    scene.add ( athelete[body_parts.body] );

    bone_helper = new THREE.SkeletonHelper(athelete[body_parts.body]);
    scene.add (bone_helper);

    initBikePosition();

    body_part_cnt++;

    //onRotateCamera(0);
};

function initBikePosition(){
    setUpperBody();
    setArms();
    setRLeg(45);
    setLLeg(95);
}

function onRotateCamera(value) {
    moveCamera (value, 110 , -5)
    //var rad = ToRad * value ;
    //camera.position.x = Math.cos(rad) * 10;
    //camera.position.z = Math.sin (rad) * 10;
    //camera.lookAt ( scene.position );
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

    //var pos = new THREE.Vector3(x, y, z)
    //var quat = new THREE.Quaternion(q1, q2, q3, q4);

    //console.log("update body part ", body_part);

    //athelete[body_part].position.copy(pos);
    //athelete[body_part].quaternion.copy(quat);

    //console.log("updated body part ", body_part);
}

function paintGL(canvas) {

    //var timer = Date.now() * 0.0005;
    //camera.position.x = Math.cos(timer) * 10;
    //camera.position.z = Math.sin (timer) * 10;
    //camera.lookAt ( scene.position );

    if( isAtheleteLoaded){
        moveLegs();
        bone_helper.update ();
    }

    renderer.render( scene, camera );
}

var step = 4;
function moveLegs(){
    // Is moving update
    if (currDirectionUp){
        currAngle += step;
        if (currAngle > maxLegAngle){
            currAngle = maxLegAngle;
            currDirectionUp = false;
        }
    }else{
        currAngle -= step;
        if (currAngle < minLegAngle){
            currAngle = minLegAngle;
            currDirectionUp = true;
        }
    }

    var rightLeg = currAngle;
    // Opposite
    var leftLeg = (maxLegAngle - currAngle) +  minLegAngle;

    setRLeg(rightLeg);
    setLLeg(leftLeg);
}

function setUpperBody(){
    var mtx = new THREE.Matrix4();
    var mtx2 = new THREE.Matrix4();
    var pos = new THREE.Vector3();
    var quat = new THREE.Quaternion();

    mtx.makeTranslation( 1, 1, 1);
    var angle0 = 25 * ToRad;//Math.sin (20 * ToRad);// * ToRad;
    var angle1 = -35 * ToRad;// Math.sin (-45 * ToRad);// * ToRad;

    mtx2.makeRotationX(angle0);
    mtx.multiply( mtx2 );

    pos.setFromMatrixPosition( mtx );
    quat.setFromRotationMatrix( mtx );

    var hips = getBone ('hips');
    var spine = getBone ('spine');
    var chest = getBone ('chest');

    if (hips !== null){
        hips.rotation.setFromRotationMatrix(mtx);
    }

    angle0 = 30 * ToRad;
    mtx2 = new THREE.Matrix4();
    mtx.makeTranslation( 1, 1, 1);
    mtx2.makeRotationX(angle0);
    mtx.multiply( mtx2 );

    if (spine !== null){
        spine.rotation.setFromRotationMatrix(mtx);
        //chest.rotation.setFromRotationMatrix(mtx);
    }

    mtx2 = new THREE.Matrix4();
    mtx.makeTranslation( 1, 1, 1);
    mtx2.makeRotationX(angle1);
    mtx.multiply( mtx2 );
    var neck = getBone ('neck');
    var head = getBone ('head');
    if (head !== null){
        head.rotation.setFromRotationMatrix(mtx);
    }
}

function setArms(){
    var mtx = new THREE.Matrix4();
    var mtx2 = new THREE.Matrix4();
    var pos = new THREE.Vector3();
    var quat = new THREE.Quaternion();

    var armR = getBone ('upper_arm.R');
    var armL = getBone ('upper_arm.L');

    mtx.makeTranslation( 1, 1, 1);
    var angle0 = -90 * ToRad;// Math.cos (180 * ToRad);// * ToRad;

    mtx2.makeRotationX(angle0);
    mtx.multiply( mtx2 );

    pos.setFromMatrixPosition( mtx );
    quat.setFromRotationMatrix( mtx );

    if (armR !== null){
        armR.rotation.setFromRotationMatrix(mtx);
        armL.rotation.setFromRotationMatrix(mtx);
    }
}

function setRLeg(angle){
    var mtx = new THREE.Matrix4();
    var mtx2 = new THREE.Matrix4();
    var pos = new THREE.Vector3();
    var quat = new THREE.Quaternion();

    var thighR = getBone ('thigh.R');
    var shinR = getBone ('shin.R');

    mtx.makeTranslation( 1, 4, 10);
    var angle0 = -angle * ToRad;
    var angle1 = angle * ToRad;

    mtx2.makeRotationX(angle0);
    mtx.multiply( mtx2 );

    pos.setFromMatrixPosition( mtx );
    quat.setFromRotationMatrix( mtx );


    if (thighR !== null){
        thighR.rotation.setFromRotationMatrix(mtx);
    }

    mtx2 = new THREE.Matrix4();
    mtx.makeTranslation( 1, 1, 1);
    mtx2.makeRotationX(angle1);
    mtx.multiply( mtx2 );

    if (shinR !== null){
        shinR.rotation.setFromRotationMatrix(mtx);
    }
}

function setLLeg(angle){
    var mtx = new THREE.Matrix4();
    var mtx2 = new THREE.Matrix4();
    var pos = new THREE.Vector3();
    var quat = new THREE.Quaternion();

    var thighL = getBone ('thigh.L');
    var shinL = getBone ('shin.L');


    mtx.makeTranslation( 1, 4, 10);
    var angle0 = -angle * ToRad;
    var angle1 = angle * ToRad;

    mtx2.makeRotationX(angle0);
    mtx.multiply( mtx2 );

    pos.setFromMatrixPosition( mtx );
    quat.setFromRotationMatrix( mtx );


    if (thighL !== null){
        thighL.rotation.setFromRotationMatrix(mtx);
    }

    mtx2 = new THREE.Matrix4();
    mtx.makeTranslation( 1, 1, 1);
    mtx2.makeRotationX(angle1);
    mtx.multiply( mtx2 );
    if (shinL !== null){
        shinL.rotation.setFromRotationMatrix(mtx);
    }

}

function getBone (name){
    for (var i = 0, len = bone_helper.bones.length; i < len; i++) {
        if (bone_helper.bones[i].name === name){
            return bone_helper.bones[i];
        }
    }
    return null;
}

