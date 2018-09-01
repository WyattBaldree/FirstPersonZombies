// Fill out your copyright notice in the Description page of Project Settings.

#include "Window.h"

AWindow::AWindow() {
	EntryPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Entry Point"));
	ExitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Exit Point"));
	AttackPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Attack Point"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}


